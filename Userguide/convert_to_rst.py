#!/usr/bin/env python3
"""Convert OpenXiL_Userguide.md to Sphinx RST with chapter splitting."""

import re
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(SCRIPT_DIR, "OpenXiL_Userguide.md")
DST = os.path.join(SCRIPT_DIR, "rst")

with open(SRC, "r", encoding="utf-8") as f:
    all_lines = f.readlines()

CHAPTERS = {
    2:  (307, 394),
    3:  (395, 837),
    4:  (838, 870),
    5:  (871, 1034),
    6:  (1035, 1210),
    7:  (1211, 1241),
    8:  (1242, 1784),
    9:  (1785, 3260),
    10: (3261, 6491),
    11: (6492, 7801),
    12: (7802, 7996),
}

RST_CHARS = ['=', '-', '~', '^', '+']


# ============================================================
# Utilities
# ============================================================

def strip_anchors(text):
    return re.sub(r"<a name='[^']*'></a>", "", text)


def strip_numbering(title):
    """Remove leading numbering like '2.1. ', '5.2.1. ', '10.1.1. '."""
    return re.sub(r'^[\d]+\.[\d.]*\s*', '', title).strip()


def escape_rst_inline(text):
    """Escape RST special characters in inline text, preserving intended formatting.

    First convert markdown escape sequences to their literal equivalents,
    then escape RST-special characters that remain.
    """
    # Convert markdown escape sequences to literals
    # In the MD file, \" is backslash+quote (0x5c 0x22).
    # Python '\\\"' = backslash(0x5c) + quote(0x22), which matches.
    text = text.replace('\\\"', '"')
    text = text.replace("\\'", "'")
    text = text.replace('\\*', '*')
    text = text.replace('\\>', '>')
    text = text.replace('\\!', '!')
    text = text.replace('\\\\', '\\')

    # Don't escape inside inline literals (``...``)
    parts = re.split(r'(``[^`]+``)', text)
    result = []
    for i, part in enumerate(parts):
        if part.startswith('``'):
            result.append(part)
        else:
            # Escape * only if not forming a ** pair (bold)
            # Easier: escape lone * but leave ** alone
            part = re.sub(r'(?<!\*)\*(?!\*)', '\\*', part)
            part = part.replace('|', '\\|')
            part = part.replace('<', '\\<')
            result.append(part)
    return ''.join(result)


def fix_image_path(src):
    """Fix image path relative to the rst directory."""
    if src.startswith('./Images/'):
        return '../Images/' + src[9:]
    elif src.startswith('Images/'):
        return '../Images/' + src[7:]
    return src


def unescape_md_code(text):
    """Unescape markdown escape sequences in code block content."""
    text = text.replace('\\\"', '"')
    text = text.replace("\\'", "'")
    text = text.replace('\\*', '*')
    text = text.replace('\\>', '>')
    text = text.replace('\\!', '!')
    text = text.replace('\\\\', '\\')
    return text


def md_inline(text):
    """Convert markdown inline formatting to RST."""
    text = strip_anchors(text)
    text = re.sub(r'`([^`]+)`', r'``\1``', text)
    text = re.sub(r'\[([^\]]+)\]\([^)]*\)', r'\1', text)
    return text


def get_heading(line):
    """Return (level, raw_title) if line is a heading, else None."""
    hm = re.match(r'^(#{1,6})\s+(.*)', line.strip())
    if hm:
        level = len(hm.group(1))
        title = strip_anchors(hm.group(2).strip())
        title = re.sub(r'\s*#+\s*$', '', title).strip()
        return level, title
    return None


def sanitize_filename(name):
    name = re.sub(r'[\[\]\\\/\(\)\*\|:"\?]', '', name)
    name = re.sub(r'\s+', '_', name)
    name = name.strip('_')
    return name.lower()


def extract_cmd_name(title):
    """Extract command name from title, stripping parameters.

    E.g. 'START_RAMPE (filename.gen)' -> 'START_RAMPE'
         'START_RECORDER / START_REC (filename.cfg)' -> 'START_RECORDER'
         'Comment */ or ;' -> 'Comment'
    """
    # Split on '(' or '/' and take first part
    name = re.split(r'\s*[\(/]', title)[0].strip()
    # Remove trailing special chars like * or ;
    name = re.sub(r'[\*;]$', '', name).strip()
    return name


def is_table_line(s):
    return s.startswith('|') and s.endswith('|') and len(s) > 2


def is_table_sep(s):
    return bool(re.match(r'^\|[\s:\-]+(\|[\s:\-]+)+\|$', s))


def convert_table(table_lines):
    rows = []
    for line in table_lines:
        s = line.strip()
        if is_table_sep(s):
            continue
        cells = [c.strip() for c in s.split('|')]
        cells = [c for c in cells if c != '']
        if cells:
            rows.append(cells)
    if not rows:
        return []
    ncols = max(len(r) for r in rows)
    widths = [0] * ncols
    for row in rows:
        for i, c in enumerate(row[:ncols]):
            widths[i] = max(widths[i], len(c))
    widths = [max(w, 4) for w in widths]
    sep = '  '.join('=' * w for w in widths)
    out = [sep]
    for i, row in enumerate(rows):
        padded = []
        for j in range(ncols):
            cell = row[j] if j < len(row) else ''
            padded.append(cell.ljust(widths[j]))
        out.append('  '.join(padded))
        if i == 0:
            out.append(sep)
    out.append(sep)
    return out


# ============================================================
# Main MD -> RST converter
# ============================================================

def md_to_rst_lines(lines, heading_offset=0, skip_first_heading=False):
    """Convert MD lines to RST.

    heading_offset: shift all heading levels by this amount.
                    E.g. heading_offset=-1 means ## becomes -, ### becomes ~, etc.
    skip_first_heading: if True, skip the very first heading encountered.
    """
    out = []
    i = 0
    n = len(lines)
    first_heading_skipped = False

    while i < n:
        line = lines[i].rstrip('\n').rstrip('\r')
        stripped = line.strip()

        # Skip HTML comments
        if stripped.startswith('<!--'):
            i += 1
            while i < n and '-->' not in lines[i]:
                i += 1
            i += 1
            continue

        # Tables
        if is_table_line(stripped):
            tbl = []
            while i < n and is_table_line(lines[i].strip()):
                tbl.append(lines[i].rstrip('\n').rstrip('\r'))
                i += 1
            rst_tbl = convert_table(tbl)
            out.append('')
            out.extend(rst_tbl)
            out.append('')
            continue

        # Fenced code block ```
        if stripped.startswith('```'):
            i += 1
            lang = stripped[3:].strip()
            out.append('')
            if lang:
                out.append(f'.. code-block:: {lang}')
                out.append('')
            while i < n and not lines[i].strip().startswith('```'):
                cl = lines[i].rstrip('\n').rstrip('\r')
                out.append('   ' + cl if cl.strip() else '')
                i += 1
            i += 1
            out.append('')
            continue

        # Indented code block (4 spaces) - but only if it looks like actual code
        # (not headings, images, tables that follow)
        if line.startswith('    ') and stripped:
            code_lines = []
            while i < n and (lines[i].startswith('    ') or lines[i].strip() == ''):
                cl = lines[i].rstrip('\n').rstrip('\r')
                if cl.strip():
                    code_lines.append(cl[4:])
                else:
                    code_lines.append('')
                i += 1
                if i < n:
                    nxt = lines[i].strip()
                    if nxt.startswith('#') or nxt.startswith('![') or nxt.startswith('|'):
                        break
            # Wrap in code-block directive
            out.append('')
            out.append('.. code-block::')
            out.append('')
            for cl in code_lines:
                out.append('   ' + cl if cl.strip() else '')
            out.append('')
            continue

        # Headings
        hm = re.match(r'^(#{1,6})\s+(.*)', stripped)
        if hm:
            level = len(hm.group(1))
            raw_title = strip_anchors(hm.group(2).strip())
            raw_title = re.sub(r'\s*#+\s*$', '', raw_title).strip()
            clean_title = strip_numbering(raw_title)

            if skip_first_heading and not first_heading_skipped:
                first_heading_skipped = True
                i += 1
                continue

            effective_level = max(1, level + heading_offset)

            char = RST_CHARS[min(effective_level - 1, len(RST_CHARS) - 1)]
            out.append('')
            out.append(clean_title)
            out.append(char * max(len(clean_title), 4))
            out.append('')
            i += 1
            continue

        # Images
        im = re.match(r'^!\[([^\]]*)\]\(([^)]+)\)', stripped)
        if im:
            src = fix_image_path(im.group(2))
            alt = im.group(1)
            out.append('')
            out.append(f'.. image:: {src}')
            if alt:
                out.append(f'   :alt: {alt}')
            out.append('')
            i += 1
            continue

        # Escape RST special chars in body text
        text = md_inline(stripped)
        text = escape_rst_inline(text)
        out.append(text)
        i += 1
    return out


# ============================================================
# Specialized converter for function files (ch10)
# ============================================================

def func_md_to_rst(lines):
    """Convert function MD lines to RST with proper code-block for C/VB."""
    out = []
    i = 0
    n = len(lines)

    while i < n:
        line = lines[i].rstrip('\n').rstrip('\r')
        stripped = line.strip()

        # Skip the first heading (it becomes the file title)
        hm = get_heading(stripped)
        if hm:
            i += 1
            continue

        # Skip HTML comments
        if stripped.startswith('<!--'):
            i += 1
            while i < n and '-->' not in lines[i]:
                i += 1
            i += 1
            continue

        # Detect "**C:**" or "**VB:**" language markers followed by indented code
        lang_match = re.match(r'^\*\*(C|VB|Python|Perl):?\*\*\s*$', stripped)
        if lang_match:
            lang = lang_match.group(1)
            i += 1
            # Skip blank lines
            while i < n and lines[i].strip() == '':
                i += 1
            # Collect indented code lines
            code_lines = []
            while i < n and (lines[i].startswith('    ') or lines[i].strip() == ''):
                cl = lines[i].rstrip('\n').rstrip('\r')
                if cl.strip():
                    code_lines.append(cl)
                i += 1
                if i < n and (lines[i].strip().startswith('**') or
                              get_heading(lines[i]) is not None or
                              lines[i].strip().startswith('Return value') or
                              lines[i].strip() == ''):
                    # Check if next non-blank line is a heading or language marker
                    next_stripped = lines[i].strip()
                    if next_stripped.startswith('**') or get_heading(lines[i]) is not None:
                        break
            if code_lines:
                rst_lang = 'c' if lang == 'C' else ('vb.net' if lang == 'VB' else lang.lower())
                out.append('')
                out.append(f'.. code-block:: {rst_lang}')
                out.append('')
                for cl in code_lines:
                    out.append('   ' + unescape_md_code(cl.strip()))
                out.append('')
            continue

        # Tables
        if is_table_line(stripped):
            tbl = []
            while i < n and is_table_line(lines[i].strip()):
                tbl.append(lines[i].rstrip('\n').rstrip('\r'))
                i += 1
            rst_tbl = convert_table(tbl)
            out.append('')
            out.extend(rst_tbl)
            out.append('')
            continue

        # Fenced code block
        if stripped.startswith('```'):
            i += 1
            lang = stripped[3:].strip()
            out.append('')
            if lang:
                out.append(f'.. code-block:: {lang}')
                out.append('')
            while i < n and not lines[i].strip().startswith('```'):
                cl = lines[i].rstrip('\n').rstrip('\r')
                out.append('   ' + unescape_md_code(cl) if cl.strip() else '')
                i += 1
            i += 1
            out.append('')
            continue

        # Indented code (non-language-marked)
        if line.startswith('    ') and stripped:
            code_lines = []
            while i < n and (lines[i].startswith('    ') or lines[i].strip() == ''):
                cl = lines[i].rstrip('\n').rstrip('\r')
                if cl.strip():
                    code_lines.append(cl[4:])
                else:
                    code_lines.append('')
                i += 1
                if i < n:
                    nxt = lines[i].strip()
                    if nxt.startswith('#') or nxt.startswith('![') or nxt.startswith('|'):
                        break
            if code_lines:
                out.append('')
                out.append('.. code-block::')
                out.append('')
                for cl in code_lines:
                    out.append('   ' + cl if cl.strip() else '')
                out.append('')
            continue

        # Images
        im = re.match(r'^!\[([^\]]*)\]\(([^)]+)\)', stripped)
        if im:
            src = fix_image_path(im.group(2))
            alt = im.group(1)
            out.append('')
            out.append(f'.. image:: {src}')
            if alt:
                out.append(f'   :alt: {alt}')
            out.append('')
            i += 1
            continue

        # Regular text
        text = md_inline(stripped)
        text = escape_rst_inline(text)
        out.append(text)
        i += 1
    return out


# ============================================================
# Specialized converter for script command files (ch9)
# ============================================================

def script_cmd_md_to_rst(title, lines):
    """Convert script command MD lines to RST with prototype in code-block.

    title: the raw heading text, e.g. 'START_RAMPE (filename.gen)'
    lines: the body lines after the heading
    """
    out = []
    # Add the command prototype as a code block at the top
    out.append('.. code-block::')
    out.append('')
    out.append(f'   {title}')
    out.append('')

    i = 0
    n = len(lines)
    while i < n:
        line = lines[i].rstrip('\n').rstrip('\r')
        stripped = line.strip()

        # Skip the first heading (it's the same as title)
        hm = get_heading(stripped)
        if hm:
            i += 1
            continue

        # Skip HTML comments
        if stripped.startswith('<!--'):
            i += 1
            while i < n and '-->' not in lines[i]:
                i += 1
            i += 1
            continue

        # Tables
        if is_table_line(stripped):
            tbl = []
            while i < n and is_table_line(lines[i].strip()):
                tbl.append(lines[i].rstrip('\n').rstrip('\r'))
                i += 1
            rst_tbl = convert_table(tbl)
            out.append('')
            out.extend(rst_tbl)
            out.append('')
            continue

        # Fenced code block
        if stripped.startswith('```'):
            i += 1
            lang = stripped[3:].strip()
            out.append('')
            if lang:
                out.append(f'.. code-block:: {lang}')
                out.append('')
            while i < n and not lines[i].strip().startswith('```'):
                cl = lines[i].rstrip('\n').rstrip('\r')
                out.append('   ' + cl if cl.strip() else '')
                i += 1
            i += 1
            out.append('')
            continue

        # Indented code
        if line.startswith('    ') and stripped:
            code_lines = []
            while i < n and (lines[i].startswith('    ') or lines[i].strip() == ''):
                cl = lines[i].rstrip('\n').rstrip('\r')
                if cl.strip():
                    code_lines.append(cl[4:])
                else:
                    code_lines.append('')
                i += 1
                if i < n:
                    nxt = lines[i].strip()
                    if nxt.startswith('#') or nxt.startswith('![') or nxt.startswith('|'):
                        break
            if code_lines:
                out.append('')
                out.append('.. code-block::')
                out.append('')
                for cl in code_lines:
                    out.append('   ' + cl if cl.strip() else '')
                out.append('')
            continue

        # Images
        im = re.match(r'^!\[([^\]]*)\]\(([^)]+)\)', stripped)
        if im:
            src = fix_image_path(im.group(2))
            alt = im.group(1)
            out.append('')
            out.append(f'.. image:: {src}')
            if alt:
                out.append(f'   :alt: {alt}')
            out.append('')
            i += 1
            continue

        # Regular text
        text = md_inline(stripped)
        text = escape_rst_inline(text)
        out.append(text)
        i += 1
    return out


# ============================================================
# File writers
# ============================================================

def write_rst_file(path, title, rst_lines, label=None):
    with open(path, 'w', encoding='utf-8') as f:
        if label:
            f.write(f'.. _{label}:\n\n')
        f.write(title + '\n')
        f.write('=' * max(len(title), 4) + '\n\n')
        for rl in rst_lines:
            f.write(rl + '\n')


def write_chapter_file(path, title, rst_lines, toctree_entries=None, label=None):
    with open(path, 'w', encoding='utf-8') as f:
        if label:
            f.write(f'.. _{label}:\n\n')
        f.write(title + '\n')
        f.write('=' * max(len(title), 4) + '\n\n')
        for rl in rst_lines:
            f.write(rl + '\n')
        if toctree_entries:
            f.write('\n.. toctree::\n')
            f.write('   :maxdepth: 2\n\n')
            for entry in toctree_entries:
                f.write(f'   {entry}\n')


# ============================================================
# Create directories
# ============================================================
os.makedirs(DST, exist_ok=True)
ch9_dir = os.path.join(DST, "command")
ch10_dir = os.path.join(DST, "function")
os.makedirs(ch9_dir, exist_ok=True)
os.makedirs(ch10_dir, exist_ok=True)

# ============================================================
# Chapters 2-8, 11-12: single-file chapters
# ============================================================
CH_NAMES = {
    2: 'what_is_openxilenv',
    3: 'installation',
    4: 'concept',
    5: 'control_panel',
    6: 'display_of_variables',
    7: 'calibration_tree',
    8: 'processes',
    11: 'appendix',
    12: 'openxilenv_for_hil',
}

CH_TITLES = {
    2: 'What is OpenXiLEnv',
    3: 'Installation',
    4: 'Concept',
    5: 'Control Panel',
    6: 'Display of variables',
    7: 'Calibration tree',
    8: 'Processes',
    11: 'Appendix',
    12: 'OpenXiL-Env for HiL',
}

for ch_num in [2, 3, 4, 5, 6, 7, 8, 11, 12]:
    start, end = CHAPTERS[ch_num]
    ch_lines = all_lines[start - 1:end]
    # heading_offset=-1: ## -> =, ### -> -, #### -> ~, ##### -> ^
    # skip_first_heading=True: the first ## heading is the chapter title, already written by file header
    rst = md_to_rst_lines(ch_lines, heading_offset=-1, skip_first_heading=True)
    fname = CH_NAMES[ch_num] + '.rst'
    title = CH_TITLES[ch_num]
    with open(os.path.join(DST, fname), 'w', encoding='utf-8') as f:
        f.write(f'.. _chapter_{CH_NAMES[ch_num]}:\n\n')
        f.write(title + '\n')
        f.write('=' * max(len(title), 4) + '\n\n')
        for rl in rst:
            f.write(rl + '\n')
    print(f"Chapter {ch_num}: {fname}")


# ============================================================
# Chapter 9: Script language
# ============================================================
ch9_start, ch9_end = CHAPTERS[9]
ch9_lines = all_lines[ch9_start - 1:ch9_end]

headings_9 = []
for idx, line in enumerate(ch9_lines):
    h = get_heading(line)
    if h:
        headings_9.append((idx, h[0], h[1]))


def find_heading_idx(target_title_pattern, min_level=3, max_level=4):
    for idx, level, title in headings_9:
        if min_level <= level <= max_level and target_title_pattern in title:
            return idx
    return None


idx_9_2_1 = find_heading_idx('9.2.1', 4, 4)
idx_9_2_2 = find_heading_idx('9.2.2', 4, 4)


def extract_section(match_func):
    """Extract lines for a ### section."""
    result = []
    inside = False
    for idx, line in enumerate(ch9_lines):
        h = get_heading(line)
        if h and h[0] == 3 and match_func(h[1]):
            inside = True
        elif h and h[0] == 3 and inside:
            break
        if inside:
            result.append(line.rstrip('\n').rstrip('\r'))
    return result


intro_9 = extract_section(lambda t: '9.1' in t)
notes_9 = extract_section(lambda t: '9.3' in t)
dbg_9 = extract_section(lambda t: '9.4' in t)
other_9 = extract_section(lambda t: '9.5' in t)

# Extract script commands (##### in section 9.2.1)
script_commands = []
current_title = None
current_lines = []
in_basic = False

for idx, line in enumerate(ch9_lines):
    h = get_heading(line)
    if h and h[0] == 4 and idx_9_2_1 is not None and idx == idx_9_2_1:
        in_basic = True
        continue
    if h and h[0] == 4 and idx_9_2_2 is not None and idx == idx_9_2_2:
        in_basic = False
        break
    if in_basic:
        if h and h[0] == 5:
            if current_title is not None:
                script_commands.append((current_title, current_lines))
            current_title = h[1]
            current_lines = [line.rstrip('\n').rstrip('\r')]
        elif current_title is not None:
            current_lines.append(line.rstrip('\n').rstrip('\r'))

if current_title is not None:
    script_commands.append((current_title, current_lines))

# Write each script command file
cmd_stems = []
for title, cmd_lines in script_commands:
    clean_title = strip_numbering(title)
    cmd_name = extract_cmd_name(clean_title)
    fname = sanitize_filename(cmd_name) + '.rst'
    rst = script_cmd_md_to_rst(clean_title, cmd_lines)
    label = 'command_' + sanitize_filename(cmd_name)
    write_rst_file(os.path.join(ch9_dir, fname), cmd_name, rst, label)
    cmd_stems.append(sanitize_filename(cmd_name))

# Write support section files
intro_rst = md_to_rst_lines(intro_9, heading_offset=-1, skip_first_heading=True)
write_rst_file(os.path.join(ch9_dir, 'introduction.rst'), 'Introduction', intro_rst, 'command_intro')

notes_rst = md_to_rst_lines(notes_9, heading_offset=-1, skip_first_heading=True)
write_rst_file(os.path.join(ch9_dir, 'notes.rst'), 'Notes on the instructions', notes_rst, 'command_notes')

dbg_rst = md_to_rst_lines(dbg_9, heading_offset=-1, skip_first_heading=True)
write_rst_file(os.path.join(ch9_dir, 'script_debug_window.rst'), 'Script debug window', dbg_rst, 'command_debug_window')

other_rst = md_to_rst_lines(other_9, heading_offset=-1, skip_first_heading=True)
write_rst_file(os.path.join(ch9_dir, 'other.rst'), 'Other', other_rst, 'command_other')

# Write top-level chapter 9 file
preamble_9 = []
for idx, line in enumerate(ch9_lines):
    h = get_heading(line)
    if h and h[0] == 3:
        break
    preamble_9.append(line.rstrip('\n').rstrip('\r'))

preamble_rst = md_to_rst_lines(preamble_9, heading_offset=-1, skip_first_heading=True)
toctree_9 = (
    ['command/introduction'] +
    [f'command/{s}' for s in cmd_stems] +
    ['command/notes', 'command/script_debug_window', 'command/other']
)

write_chapter_file(
    os.path.join(DST, 'command.rst'),
    'Script language',
    preamble_rst,
    toctree_9,
    label='chapter_command'
)

print(f"Chapter 9: command.rst + {len(script_commands)} command files")


# ============================================================
# Chapter 10: Remote Control
# ============================================================
ch10_start, ch10_end = CHAPTERS[10]
ch10_lines = all_lines[ch10_start - 1:ch10_end]

# Extract preamble (before first ###)
preamble_10 = []
for line in ch10_lines:
    h = get_heading(line)
    if h and h[0] == 3:
        break
    preamble_10.append(line.rstrip('\n').rstrip('\r'))

# Extract individual #### functions
functions = []
current_title = None
current_lines = []

for idx, line in enumerate(ch10_lines):
    h = get_heading(line)
    if h and h[0] == 4:
        if current_title is not None:
            functions.append((current_title, current_lines))
        current_title = h[1]
        current_lines = [line.rstrip('\n').rstrip('\r')]
    elif current_title is not None:
        current_lines.append(line.rstrip('\n').rstrip('\r'))

if current_title is not None:
    functions.append((current_title, current_lines))

# Write each function file
func_stems = []
for title, func_lines in functions:
    clean_title = strip_numbering(title)
    fname = sanitize_filename(clean_title) + '.rst'
    rst = func_md_to_rst(func_lines)
    label = 'function_' + sanitize_filename(clean_title)
    write_rst_file(os.path.join(ch10_dir, fname), clean_title, rst, label)
    func_stems.append(sanitize_filename(clean_title))

# Write top-level chapter 10 file
preamble_rst_10 = md_to_rst_lines(preamble_10, heading_offset=-1, skip_first_heading=True)
toctree_10 = [f'function/{s}' for s in func_stems]

write_chapter_file(
    os.path.join(DST, 'function.rst'),
    'Remote Control',
    preamble_rst_10,
    toctree_10,
    label='chapter_function'
)

print(f"Chapter 10: function.rst + {len(functions)} function files")
print("\nDone!")

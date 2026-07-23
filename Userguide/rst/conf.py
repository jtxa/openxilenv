# Configuration file for the Sphinx documentation builder.

project = 'OpenXiL Environment'
copyright = '2024, dSpace GmbH'
author = 'Eric Bieber, Udo Gillich, Horst Kiebler, Michael Matthäi'
version = '0.8.15'
release = '0.8.15'

extensions = []

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

html_theme = 'alabaster'
html_static_path = ['_static']

html_theme_options = {
    'description': 'OpenXiL Environment User Guide',
    'github_user': '',
    'github_repo': '',
}

html_sidebars = {
    '**': [
        'about.html',
        'navigation.html',
        'relations.html',
        'searchbox.html',
    ]
}

rst_prolog = """
.. |br| raw:: html

   <br>
"""

# -- Custom role definitions for :function: and :script_command: -----------

from docutils import nodes
from docutils.parsers.rst import roles
from sphinx import addnodes


def function_role(name, rawtext, text, lineno, inliner, options=None, content=None):
    """Custom role for function references like :function:`XilEnv_GetEnvironVar`"""
    options = options or {}
    content = content or []
    node = addnodes.pending_xref(
        '', refdomain='std', reftype='ref',
        reftarget='function_' + text.lower(), refwarn=False,
        refexplicit=False, refdoc=inliner.document.settings.env.docname)
    node += nodes.inline(rawtext, text, classes=['xref', 'function'])
    return [node], []


def command_role(name, rawtext, text, lineno, inliner, options=None, content=None):
    """Custom role for command references like :script_command:`ADD_BBVARI`"""
    options = options or {}
    content = content or []
    node = addnodes.pending_xref(
        '', refdomain='std', reftype='ref',
        reftarget='command_' + text.lower(), refwarn=False,
        refexplicit=False, refdoc=inliner.document.settings.env.docname)
    node += nodes.inline(rawtext, text, classes=['xref', 'command'])
    return [node], []


roles.register_local_role('function', function_role)
roles.register_local_role('script_command', command_role)
roles.register_local_role('cmd', command_role)

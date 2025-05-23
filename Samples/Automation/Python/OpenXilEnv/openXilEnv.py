import os
import subprocess
import sys
import time

from CoreTestFramework.OpenXilEnv.XilEnvRpc import *


class OpenXilEnv:

    def __init__(self, xilEnvPath):
        self.__xilEnvDllPath = os.path.join(xilEnvPath, "XilEnvRpc.dll")
        self.__xilEnvExePath = os.path.join(xilEnvPath, "XilEnvGui.exe")

        self.__xilEnvProcess = None

        self.__xilEnv = XilEnvRpc(self.__xilEnvDllPath)

        self.__defaultXilEnvVariables = [
            "XilEnv.CycleCounter",
            "XilEnv.EquationCalculator",
            "XilEnv.exit",
            "XilEnv.ExitCode",
            "XilEnv.Generator",
            "XilEnv.Realtime",
            "XilEnv.RealtimeFactor",
            "XilEnv.SampleFrequency",
            "XilEnv.SampleTime",
            "XilEnv.Script",
            "XilEnv.StimulusPlayer",
            "XilEnv.StimulusPlayer.suspend",
            "XilEnv.TraceRecorder",
            "XilEnv.Version",
            "XilEnv.Version.Patch"
        ]

        self.__attachedVar = {}

    def __attachDefaultXilEnvVariables(self):
        for variableName in self.__defaultXilEnvVariables:
            variableId = self.__xilEnv.AttachVari(variableName)
            if variableId > 0:
                self.__attachedVar[variableName] = variableId
            else:
                print(f"Could not attach variable {variableName}")

    def __start(self, iniFilePath, startWithGUI):
        try:
            if startWithGUI:
                self.__xilEnvProcess = subprocess.Popen([self.__xilEnvExePath, "-ini", iniFilePath],
                                                        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            else:
                self.__xilEnvProcess = subprocess.Popen([self.__xilEnvExePath, "-ini", iniFilePath, "-nogui"],
                                                        stdout=subprocess.PIPE, stderr=subprocess.PIPE)

            self.__waitUntilApplicationIsReady()

            connectionStatus = self.__xilEnv.ConnectTo("")
            if connectionStatus == 0:
                print("Python has been successfully connected to XilEnv")
                self.__attachDefaultXilEnvVariables()
                return
            else:
                print("Could not connect to openXilEnv")
                sys.exit(1)
        except Exception as e:
            print(e)
            sys.exit(1)

    def __translateSecondsToCycleCounts(self, seconds):
        sampleFrequency = self.readSignal("XilEnv.SampleFrequency")
        return int(seconds * sampleFrequency)

    def __waitUntilApplicationIsReady(self):
        time.sleep(5)

    def attachVariables(self, signalNames):
        for variableName in signalNames:
            variableId = self.__xilEnv.AttachVari(variableName)
            if variableId > 0:
                self.__attachedVar[variableName] = variableId
            else:
                print(f"Could not attach variable {variableName}")

    def disconnectAndCloseXil(self):
        if self.__xilEnvProcess:
            self.__xilEnv.DisconnectAndClose(0, 0)

    def readMultipleSignals(self, signalNames):
        signalIdList = [self.__attachedVar[name] for name in signalNames]
        signalValues = self.__xilEnv.ReadFrame(signalIdList)

        return signalValues

    def readSignal(self, signalName):
        value = self.__xilEnv.Get(self.__attachedVar[signalName])

        return value

    def removeVariables(self):
        for signalName, variableId in self.__attachedVar.items():
            self.__xilEnv.RemoveVari(variableId)

    def waitSeconds(self, waitTimeInSec):
        cycles = self.__translateSecondsToCycleCounts(waitTimeInSec)

        self.__xilEnv.StopScheduler()
        self.__xilEnv.DoNextCyclesAndWait(cycles)
        self.__xilEnv.ContinueScheduler()

        return waitTimeInSec

    def startWithGui(self, iniFilePath):
        self.__start(iniFilePath, True)

    def startWithoutGui(self, iniFilePath):
        self.__start(iniFilePath, False)

    def switchRTFactor(self, stringValue):
        self.__xilEnv.StopScheduler()
        self.__xilEnv.ChangeSettings("NOT_FASTER_THAN_REALTIME", stringValue)
        self.__xilEnv.ContinueScheduler()

        return stringValue

    def writeMultipleSignals(self, signalNames, signalValues):
        signalIdList = [self.__attachedVar[name] for name in signalNames]
        self.__xilEnv.WriteFrame(signalIdList, signalValues)

        return signalValues

    def writeSignal(self, signalName, rawValue):
        self.__xilEnv.Set(self.__attachedVar[signalName], rawValue)

    def waitUntilValueAlmostMatch(self, signalName, expectedSignalValue, waitTimeInSec, tolerance):
        cycleCount = self.__translateSecondsToCycleCounts(waitTimeInSec)

        minValue = expectedSignalValue - tolerance
        maxValue = expectedSignalValue + tolerance
        equation = f"{signalName} >= {minValue} && {signalName} <= {maxValue}"
        remainingCycles = self.__xilEnv.WaitUntil(equation, cycleCount)

        return remainingCycles

    def waitUntilValueIsGreater(self, signalName, expectedSignalValue, waitTimeInSec):
        cycleCount = self.__translateSecondsToCycleCounts(waitTimeInSec)

        equation = f"{signalName} > {expectedSignalValue}"
        remainingCycles = self.__xilEnv.WaitUntil(equation, cycleCount)

        return remainingCycles

    def waitUntilValueIsSmaller(self, signalName, expectedSignalValue, waitTimeInSec):
        cycleCount = self.__translateSecondsToCycleCounts(waitTimeInSec)

        equation = f"{signalName} < {expectedSignalValue}"
        remainingCycles = self.__xilEnv.WaitUntil(equation, cycleCount)

        return remainingCycles

    def waitUntilValueMatch(self, signalName, expectedSignalValue, waitTimeInSec):
        cycleCount = self.__translateSecondsToCycleCounts(waitTimeInSec)

        equation = f"{signalName} == {expectedSignalValue}"
        remainingCycles = self.__xilEnv.WaitUntil(equation, cycleCount)

        return remainingCycles

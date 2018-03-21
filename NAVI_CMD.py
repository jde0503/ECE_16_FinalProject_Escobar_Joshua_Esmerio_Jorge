###############################################################################
# Authors: Joshua Escobar, PID: A11606542
#          Jorge Esmerio, PID: A11459879
# Class:  ECE 16
# Lab: Final Project
#
# File Name: stream_filters.py
# Description: Implements real-time processing of Arduino sensor data to encode
#              data as commands for blind person.
###############################################################################

# Import necessary packages and modules.
import time
import warnings
import serial
import serial.tools.list_ports
import scipy.signal as sig
import numpy as np


# Declare and initialize variables and constants.
NUM_SAMPLES = 500
SAMPLE_RATE = 150
WAIT_TIME = 0.001
FILTER_ORDER = 3  # Provided by instructor
HIGH_PASS_CUTOFF = 0.1  # Determined by examinging power spectral density.
LOW_PASS_CUTOFF = 0.5  # Determined by examining power spectral density.
NUM_WINDOWS = 100
b_high, a_high = sig.butter(FILTER_ORDER, HIGH_PASS_CUTOFF, 'highpass',
                            analog=False, output='ba')
b_low, a_low = sig.butter(FILTER_ORDER, LOW_PASS_CUTOFF, 'lowpass',
                          analog=False, output='ba')
box = sig.boxcar(NUM_WINDOWS)

FLEX_THRESH = 10
ACCEL_THRESH = 14500
leftCalfRaw = np.zeros([NUM_SAMPLES])
rightCalfRaw = np.zeros([NUM_SAMPLES])
previousOrientation = None
timeOfLastFlex = 0


# Helper function that determines Arduino orientation from accelerometer data.
def getOrientation(A_x, A_y, A_z):
    if (A_x > ACCEL_THRESH):
        return 'F'
    elif (A_y > ACCEL_THRESH):
        return 'R'
    elif (A_y < -ACCEL_THRESH):
        return 'L'
    elif (A_z > ACCEL_THRESH):
        return 'S'
    else:
        return 'S'


# Helper function that determines if flex is present in last second of EMG data.
def isFlexPresent(emgData):
    if (np.amax(emgData) > FLEX_THRESH):
        return True
    else:
        return False


# Find Arduino device and its port.
arduinoPorts = [p.device
                for p in serial.tools.list_ports.comports()
                if 'Arduino' in p.description]

if not arduinoPorts:
    raise IOError("\nNo Arduino found")

if len(arduinoPorts) > 1:
    warnings.warn('\nMultiple Arduinos found - using the first...')

arduinoSerial = serial.Serial(arduinoPorts[0], baudrate=115200, timeout=0.01)
print("\nUsing Arduino connected to port " + str(arduinoSerial.name))

# Clear input and output buffers.
arduinoSerial.reset_input_buffer()
arduinoSerial.reset_output_buffer()


# Print welcome message and instructions.
print("\nWelcome. This program interprets Arduino sensor data as commands " +
      "to be sent to a blind person")
print("You may press [Ctrl] + [C] to terminate this program at any time.\n")

# Prompt user to request data.
input("Please press [Enter] to send begin plotting: ")

try:
    # The main loop.
    while (True):
        if (arduinoSerial.in_waiting):
            # Parse serial transmission.
            arduinoData = [float(x) for x in arduinoSerial.readline().
                           decode("utf-8").strip("\n\r").split()]

            A_x, A_y, A_z, leftCalf, rightCalf = arduinoData

            # Get Arduino orientation.
            currentOrientation = getOrientation(A_x, A_y, A_z)

            # If orientation is new, send it back to Arduino as command.
            if (currentOrientation != previousOrientation):
                arduinoSerial.write(currentOrientation.encode("ascii"))
                print("Command Sent: " + currentOrientation)
                previousOrientation = currentOrientation

            # Add new EMG data to buffers and apply filters.
            leftCalfRaw[:-1] = leftCalfRaw[1:]
            rightCalfRaw[:-1] = rightCalfRaw[1:]
            leftCalfRaw[-1] = leftCalf
            rightCalfRaw[-1] = rightCalf

            leftCalfFiltered = sig.filter(b_low, a_low, leftCalfRaw)
            leftCalfFiltered = sig.filter(b_high, a_high, leftCalfFiltered)
            leftCalfFiltered = np.absolute(leftCalfFiltered)
            leftCalfFiltered = sig.lfilter(box, 1, leftCalfFiltered)

            rightCalfFiltered = sig.filter(b_low, a_low, rightCalfRaw)
            rightCalfFiltered = sig.filter(b_high, a_high, rightCalfFiltered)
            rightCalfFiltered = np.absolute(rightCalfFiltered)
            lrightCalfFiltered = sig.lfilter(box, 1, rightCalfFiltered)

            # If timing is right, check for flexes.
            if ((time.perf_counter() - timeOfLastFlex) > 1):
                leftFlexDetected = isFlexPresent(leftCalfFiltered[-150:])
                rightFlexDetected = isFlexPresent(rightCalfFiltered[-150:])

                # If flexes are present and orientation is appropriate, send
                # speed up, slow down, pick up, put down commands
                if (currentOrientation == 'F'):
                    if (leftFlexDetected):
                        arduinoSerial.write('B'.encode("ascii"))
                        timeOfLastFlex = time.perf_counter()
                        print("Command Sent: B")
                    elif (rightFlexDetected):
                        arduinoSerial.write('F'.encode("ascii"))
                        timeOfLastFlex = time.perf_counter()
                        print("Command Sent: F")
                elif (currentOrientation == 'S'):
                    if (leftFlexDetected):
                        arduinoSerial.write('D'.encode("ascii"))
                        timeOfLastFlex = time.perf_counter()
                        print("Command Sent: D")
                    elif (rightFlexDetected):
                        arduinoSerial.write('U'.encode("ascii"))
                        timeOfLastFlex = time.perf_counter()
                        print("Command Sent: U")

except KeyboardInterrupt:
    print("\nStop command received.")
finally:
    arduinoSerial.close()
    print("Goodbye.")
    exit()

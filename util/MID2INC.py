from sys import exit

# Backglass firmware constants
MAX_TRACKS = 4
NOTE_REST = 0

# Midi file verification
# These parameters should match if the MPT project was set up correctly
MIDI_FORMAT = 1
MIDI_TRACKS_MIN = 1
MIDI_TRACKS_MAX = (MAX_TRACKS)
MIDI_DIVISION = 480
MIDI_TEMPO = 66667
MIDI_NOTE_RATE = (16000000 / (1024 * 256))  # The frequency of note playing on actual hardware, ~61 Hz

# Global variables
Tracks = 0
Midi_Length = 0
Output_Data = []
Division = MIDI_DIVISION
Tempo = MIDI_TEMPO


def getMultiByteValue(bytes):
	# Returns the value of a multi-byte value starting at position 'pointer' in the midi file
	# This modifies the 'pointer' variable

	global Data
	global Pointer

	Temp_Value = 0
	for Byte in range(bytes):
		Temp_Value *= 0x100
		Temp_Value += Data[Pointer + Byte]
	Pointer += bytes
	return Temp_Value


def getVariableLengthValue():
	# Returns the variable-length value starting at position 'pointer' in the midi file
	# This modifies the 'pointer' variable

	global Data
	global Pointer

	Temp_Time = 0
	Last_Byte = Data[Pointer]
	Pointer += 1
	while ((Last_Byte & 0b10000000) > 0):
		Temp_Time = ((Temp_Time + (Last_Byte & 0b01111111)) * 0x80)
		Last_Byte = Data[Pointer]
		Pointer += 1
	Temp_Time += Last_Byte
	return Temp_Time


def getIncTime(delta_time):
	# Returns the amount of Backglass time units given a number of delta_time units
	# This uses the known values of MIDI_DIVISION and Tempo

	if ((Division == MIDI_DIVISION) and (Tempo == MIDI_TEMPO)):
		return int(delta_time / 120)

	Delta_Time_Rate = ((1000000 / Tempo) * Division)
	Delta_Time_Per_Note = (Delta_Time_Rate / MIDI_NOTE_RATE)
	return int(round(delta_time / Delta_Time_Per_Note))


def getIncNote(midi_note):
	# Returns a note value that can be read by the Backglass library, given a MIDI note

	return (midi_note + 1)


def parseMetadataTrack():
	# Parses through the first midi track, extracting useful metadata to global variables
	# This modifies the 'Pointer', 'Tempo', and 'Midi_Length' variables

	global Data
	global Pointer
	global Midi_Length
	global Tempo

	Track_Aligned = False
	while (Track_Aligned == False):
		if ((Data[Pointer] == ord('M')) and (Data[Pointer + 1] == ord('T')) and (Data[Pointer + 2] == ord('r')) and (Data[Pointer + 3] == ord('k'))):
			Pointer += 4
			Track_Aligned = True
		else:
			Pointer += 1

	Track_Length = getMultiByteValue(4)
	End_Pointer = Pointer + Track_Length

	# Handle initial rests
	Midi_Length += getVariableLengthValue()

	while (Pointer < (End_Pointer - 2)):
		if (Data[Pointer] == 0xFF):
			Pointer += 1
			if (Data[Pointer] == 0x00):    # Sequence Number (Unsupported)
				Pointer += 4
			elif (Data[Pointer] == 0x01):  # Text Event (Unsupported)
				Pointer += 1
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
			elif (Data[Pointer] == 0x02):  # Copyright Notice (Unsupported)
				Pointer += 1
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
			elif (Data[Pointer] == 0x03):  # Track Name (Unsupported)
				Pointer += 1
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
			elif (Data[Pointer] == 0x04):  # Instrument Name (Unsupported)
				Pointer += 1
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
			elif (Data[Pointer] == 0x05):  # Lyric (Unsupported)
				Pointer += 1
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
			elif (Data[Pointer] == 0x06):  # Marker (Unsupported)
				Pointer += 1
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
			elif (Data[Pointer] == 0x07):  # Cue Point (Unsupported)
				Pointer += 1
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
			elif (Data[Pointer] == 0x20):  # Channel Prefix (Unsupported)
				Pointer +=3
			elif (Data[Pointer] == 0x2F):  # End of Track
				Pointer += 1
			elif (Data[Pointer] == 0x51):  # Set Tempo
				Pointer += 2
				Tempo = getMultiByteValue(3)
			elif (Data[Pointer] == 0x54):  # SMTPE Offset (Unsupported)
				Pointer += 7
			elif (Data[Pointer] == 0x58):  # Time Signature (Unsupported)
				Pointer += 6
			elif (Data[Pointer] == 0x59):  # Key Signature (Unsupported)
				Pointer += 4
			elif (Data[Pointer] == 0x7F):  # Sequencer-Specific Meta-event (Unsupported)
				Pointer += 1
				Sysex_Length = getVariableLengthValue()
				Temp_Length = getVariableLengthValue()
				Pointer += Temp_Length
		elif (Data[Pointer] == 0xF0):    # Sequencer-Specific Meta-event (Unsupported)
			Pointer += 1
			Sysex_Length = getVariableLengthValue()
			Pointer += Sysex_Length
		else:
			Pointer += 1                   # Unknown/unsupported event-- move on until re-aligned

		Midi_Length += getVariableLengthValue()

	return


def parseMidiTrack(track):
	global Data
	global Pointer

	# Align with the beginning of the track
	Track_Aligned = False
	while (Track_Aligned == False):
		if ((Data[Pointer] == ord('M')) and (Data[Pointer + 1] == ord('T')) and (Data[Pointer + 2] == ord('r')) and (Data[Pointer + 3] == ord('k'))):
			Pointer += 4
			Track_Aligned = True
		else:
			Pointer += 1

	Track_Length = getMultiByteValue(4)
	End_Pointer = Pointer + Track_Length

	# Handle initial rest
	Delta_Time = getDeltaTime(End_Pointer)
	if (Delta_Time > 0):
		appendTrack(track, NOTE_REST, Delta_Time)

	while (Pointer < (End_Pointer - 2)):
		Midi_Command = ((Data[Pointer] & 0b11110000) >> 4)
		if (Midi_Command == 0b1000):    # Note Off
			Pointer += 3
			Note_Delta_Time = getDeltaTime(End_Pointer)
			if (getIncTime(Note_Delta_Time) > 0):
				appendTrack(track, NOTE_REST, Note_Delta_Time)
				Delta_Time += Note_Delta_Time
		elif (Midi_Command == 0b1001):  # Note On
			Pointer += 1
			Note = Data[Pointer]
			Pointer += 2
			Note_Delta_Time = getDeltaTime(End_Pointer)
			Delta_Time += Note_Delta_Time
			appendTrack(track, getIncNote(Note), Note_Delta_Time)
		else:														# End of track
			pass

	# Add extra rests until length of track matches MIDI length
	if (Delta_Time < Midi_Length):
		appendTrack(track, NOTE_REST, (Midi_Length - Delta_Time))

	# Terminate track
	appendTrack(track, NOTE_REST, 0)

	return


def getDeltaTime(end_pointer):
	# Moves through the file until reaching a "Note On", "Note Off", or "End of Track" event
	# Also stops if current Pointer is greater than end_pointer
	# Ignores all other meta and midi events
	# Returns the delta-time units elapsed in the file
	# It is expected the Pointer starts on a delta-time byte
	# This modifies the 'Pointer' variable

	global Data
	global Pointer

	Delta_Time = 0

	Ignore_Midi_Commands = True
	while (Ignore_Midi_Commands == True):
		Delta_Time += getVariableLengthValue()

		if (((Data[Pointer] & 0b11110000) == 0x80) or ((Data[Pointer] & 0b11110000) == 0x90)):
			Ignore_Midi_Commands = False
		else:
			if (Data[Pointer] == 0xFF):    # Meta Event
				Pointer += 1
				if (Data[Pointer] == 0x00):    # Sequence Number
					Pointer += 4
				elif (Data[Pointer] == 0x01):  # Text Event
					Pointer += 1
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
				elif (Data[Pointer] == 0x02):  # Copyright Notice
					Pointer += 1
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
				elif (Data[Pointer] == 0x03):  # Track Name
					Pointer += 1
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
				elif (Data[Pointer] == 0x04):  # Instrument Name
					Pointer += 1
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
				elif (Data[Pointer] == 0x05):  # Lyric
					Pointer += 1
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
				elif (Data[Pointer] == 0x06):  # Marker
					Pointer += 1
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
				elif (Data[Pointer] == 0x07):  # Cue Point
					Pointer += 1
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
				elif (Data[Pointer] == 0x20):  # Channel Prefix
					Pointer +=3
				elif (Data[Pointer] == 0x2F):  # End of Track
					Pointer += 1
					getVariableLengthValue()
					Ignore_Midi_Commands = False
				elif (Data[Pointer] == 0x51):  # Set Tempo
					Pointer += 5
				elif (Data[Pointer] == 0x54):  # SMTPE Offset
					Pointer += 7
				elif (Data[Pointer] == 0x58):  # Time Signature
					Pointer += 6
				elif (Data[Pointer] == 0x59):  # Key Signature
					Pointer += 4
				elif (Data[Pointer] == 0x7F):  # Sequencer-Specific Meta-event
					Pointer += 1
					Sysex_Length = getVariableLengthValue()
					Temp_Length = getVariableLengthValue()
					Pointer += Temp_Length
			elif (Data[Pointer] == 0xF0):    # Sequencer-Specific Meta-event
				Pointer += 1
				Sysex_Length = getVariableLengthValue()
				Pointer += Sysex_Length
			else:
				Midi_Command = ((Data[Pointer] & 0b11110000) >> 4)
				if (Midi_Command == 0b1010):    # Aftertouch
					Pointer += 3
				elif (Midi_Command == 0b1011):  # Control Change
					Pointer += 3
				elif (Midi_Command == 0b1100):  # Program Change
					Pointer += 2
				elif (Midi_Command == 0b1101):  # Channel Pressure
					Pointer += 2
				elif (Midi_Command == 0b1110):  # Pitch Bend Change
					Pointer += 3
				elif (Midi_Command == 0b1011):  # Channel Mode
					Pointer += 3
				else:
					if (Data[Pointer] == 0b11110000):  # System Exclusive Message
						while (Data[Pointer] != 0b11110111):
							Pointer += 1
						Pointer += 1
					elif (Data[Pointer] == 0b11110001):  # MIDI Time Code Quarter Frame
						Pointer += 2
					elif (Data[Pointer] == 0b11110010):  # Song Position Pointer
						Pointer += 3
					elif (Data[Pointer] == 0b11110011):  # Song Select
						Pointer += 2
					else:                                # Complete mystery
						print("")
						print("ERROR!")
						print("Unknown value found in MIDI file at byte " + str(Pointer))
						print("If output is not playable, please try exporting the midi file again.")
						Pointer += 1

		if (Pointer > end_pointer):
			Ignore_Midi_Commands = False

	return Delta_Time


def appendTrack(track, note, delta_time):
	# Appends a note (or multple notes, if the maximum note length is exceeded) to Output_Data
	# This modifies the 'Output_Data' list

	global Output_Data

	Inc_Time = getIncTime(delta_time)

	while (Inc_Time > 255):
		Output_Data[track].append([note, 255])
		Inc_Time -= 255

	Output_Data[track].append([note, Inc_Time])

	return


# Prompt for input file
Filename = input("Input file: ")
Data = open(Filename, "rb").read()
Pointer = 0;


# Check header chunk and get metadata
while (Tracks == 0):
	if ((Data[Pointer] == ord('M')) and (Data[Pointer + 1] == ord('T')) and (Data[Pointer + 2] == ord('h')) and (Data[Pointer + 3] == ord('d'))):
		Pointer += (4 + 4)
		Format = ((Data[Pointer] * 0x100) + (Data[Pointer + 1]))
		Pointer += 2
		Tracks = ((Data[Pointer] * 0x100) + (Data[Pointer + 1]) - 1)
		Pointer += 2
		Division = ((Data[Pointer] * 0x100) + (Data[Pointer + 1]))
		Pointer += 2

		# Check if MIDI file is valid
		if ((Format != MIDI_FORMAT) or (Tracks < MIDI_TRACKS_MIN) or (Tracks > MIDI_TRACKS_MAX) or (Division != MIDI_DIVISION)):
			print("")
			print("ALERT!")
			print("")
			print("The midi file provided may not be compatible with this script.")
			print("Please generate a MIDI file using OpenMPT 1.25 - 1.26, using the settings:")
			print("")
			print("- Channels: 4 (or fewer)")
			print("- Tempo Mode: Classic")
			print("- Tempo: 150")
			print("- Ticks/Row: 1")
			print("")
			print("Make sure each track is assigned an instrument.")
			if (Format == MIDI_FORMAT):
				print("")
				print("An attempt has been made to convert this file, but the output may not play.")
			input("")
			if (Format != MIDI_FORMAT):
				exit()

	else:
		Pointer += 1

# Create dummy data lists for each track
for track in range(Tracks):
	Output_Data.append([])

# Extract relevant metadata from first track
parseMetadataTrack()

# Do the actual midi parsing
for track in range(Tracks):
	parseMidiTrack(track)

# Write data to file
Temp_Filename = ""
for char in Filename:
	if (char != '.'):
		Temp_Filename += char
	else:
		break
Filename = Temp_Filename + ".inc"
Output_String = "const PROGMEM uint8_t CLIPNAME[] = {\n"
Output_String += ("\t" + str(Tracks) + ", // Number of tracks")
for track in range(Tracks):
	Output_String += ("\n\n\t// Track " + str(track) + "\n")
	Output_String += ("\t0,  // Voice type (0-3)\n")
	Output_String += ("\t0,  // Volume (0-63)\n\t")
	for note in Output_Data[track]:
		Output_String += (str(note[0]) + ", " + str(note[1]) + ", ")
Output_String = Output_String[:-2] + ("\n};\n")

Data = open(Filename, "w")
Data.write(Output_String)

print("File saved as: " + str(Filename))
print("")
print("SUCCESS!")
print("")
print("Be sure to do the following:")
print("- Copy this file into your sketch folder")
print("- Include the file at the top of your sketch using: #include \"" + Filename + "\"")
print("- Modify the file to add a clip name")
print("- Modify the file to specify track voice and volumes")
input("")

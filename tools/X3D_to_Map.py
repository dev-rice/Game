# X3D_to_Map.py
# Trevor Westphal
# converts *.x3d scenes to the custom *.map files

import sys

class Converter:
	def __init__(self):
		self.searchForObjects = False
		self.searchForTextures = False
		self.inputFileName = ""
		self.outputFileName = ""

	def convertFile(self, arguments):
		self.parseArguments(arguments)
		self.scanFile()

	def parseArguments(self, arguments):
		if(len(arguments)>4 or len(arguments)<3):
			self.printUsage()
			exit(1)

		self.inputFileName = arguments[1]
		self.outputFileName = self.inputFileName.split(".")[0]+".map"

		option1 = arguments[2]

		try:
			option2 = arguments[3]
		except:
			pass

		if(option1 == "-i" or option2 == "-i"):
			return
		if(option1 == "-so" or option2 == "-so"):
			self.searchForObjects = True
		if(option1 == "-st" or option2 == "-st"):
			self.searchForTextures = True

	def printUsage(self):
		print("X3D_to_Map usage:\n\n -so\n\tEnable searching for related objects\n -st\n\tEnable searching for related texture images\n -i\n\tIgnore related files and write placeholders\n\tThis overrides any other options\n")

	def scanFile(self):
		ifile = open(self.inputFileName, "r")

		linesLeft = 0

		for line in ifile:
			line = line.strip()
			if "<Transform DEF=\"" in line:
				if "ifs" not in line:
					linesLeft = 4
					print(line)

		ifile.close()







C = Converter()
C.convertFile(sys.argv)
# X3D_to_Map.py
# uses python3
# Trevor Westphal
# converts *.x3d scenes to the custom *.map files

import sys
import os

class ObjectReference:
	def __init__(self, name, x, y, z, scale, x_rot, y_rot, z_rot, angle):
		self.x3dName = name

		self.matchedName = ""
		self.matchedDiffTexName = ""
		self.matchedSpecTexName = ""
		self.matchedNormTexName = ""
		self.matchedEmitTexName = ""

		self.x_pos = x
		self.y_pos = y
		self.z_pos = z

		self.scale = scale

		self.x_rot = x_rot
		self.y_rot = y_rot
		self.z_rot = z_rot
		self.angle = angle

	def getDescriptor(self):
		if(self.matchedName != ""):
			name = self.matchedName
		else:
			name = self.x3dName

		if(self.matchedDiffTexName != ""):
			diff = self.matchedDiffTexName
		else:
			diff = "default_diff.png"

		if(self.matchedSpecTexName != ""):
			spec = self.matchedSpecTexName
		else:
			spec = "default_spec_norm_emit.png"

		if(self.matchedNormTexName != ""):
			norm = self.matchedNormTexName
		else:
			norm = "default_spec_norm_emit.png"

		if(self.matchedEmitTexName != ""):
			emit = self.matchedEmitTexName
		else:
			emit = "default_spec_norm_emit.png"

		# Need to convert rotation shit

		return ("%s %s %s %s %s %f %f %f %f %f %f %f\n" % (name, diff, spec, norm, emit, self.x_pos, self.y_pos, self.z_pos, self.scale, 1.0, 1.0, 1.0))



class Converter:
	def __init__(self):
		self.searchForObjects = False
		self.searchForTextures = False
		self.inputFileName = ""
		self.outputFileName = ""

	def convertFile(self, arguments):
		self.parseArguments(arguments)
		self.scanFile()
		if(self.searchForObjects):
			self.attachObjects()
		if(self.searchForTextures):
			self.attachTextures()
		self.writeToFile()		

	def parseArguments(self, arguments):
		if(len(arguments)>4 or len(arguments)<3):
			self.printUsage()
			exit(1)

		self.inputFileName = arguments[1]
		self.outputFileName = self.inputFileName.split(".")[0]+".map"

		option1 = arguments[2]
		option2 = ""

		try:
			option2 = arguments[3]
		except:
			pass

		if(option1 == "-d" or option2 == "-d"):
			return
		if(option1 == "-so" or option2 == "-so"):
			self.searchForObjects = True
		if(option1 == "-st" or option2 == "-st"):
			self.searchForTextures = True

	def printUsage(self):
		print("X3D_to_Map usage:\n\n -so\n\tEnable searching for related objects\n -st\n\tEnable searching for related texture images\n -d\n\tIgnore related files and write defaultplaceholders\n\tThis overrides any other options\n")

	def scanFile(self):
		ifile = open(self.inputFileName, "r")

		self.objectList = []

		linesLeft = 0

		for line in ifile:
			line = line.strip()
			if "<Transform DEF=\"" in line:
				if "ifs" not in line:
					linesLeft = 4
					line = line.split("\"")[1]
					if line.endswith("_TRANSFORM"):
						line = line[:-10]
					tempName = line

			if(linesLeft > 0):

				# print(line)

				if "translation" in line:
					line = line.split("\"")[1]
					line = line.split(" ")
					tempX = float(line[0])
					tempY = float(line[1])
					tempZ = float(line[2])
				elif "scale" in line:
					line = line.split("\"")[1]
					line = line.split(" ")
					tempS = float(line[0])
				elif "rotation" in line:
					line = line.split("\"")[1]
					line = line.split(" ")
					tempA = float(line[0])
					tempB = float(line[1])
					tempC = float(line[2])
					tempAngle = float(line[3])
				linesLeft-=1

				if(linesLeft==0):
					Obj = ObjectReference(tempName, tempX, tempY, tempZ, tempS, tempA, tempB, tempC, tempAngle)
					self.objectList.append(Obj);

		ifile.close()


	def attachObjects(self):
		# print("Attaching Objects:")
		for obj in self.objectList:
			for root, dirs, files in os.walk("../res/models/"):
				for file in files:
					potential_filename = os.path.join(root, file).split("/")[-1]
					if(potential_filename == obj.x3dName+".obj"):
						obj.matchedName = obj.x3dName+".obj"
						# print(obj.matchedName)


	def attachTextures(self):
		# print("Attaching textures:")
		for obj in self.objectList:
			for root, dirs, files in os.walk("../res/textures/"):
				for file in files:
					potential_filename = os.path.join(root, file).split("/")[-1]
					if(potential_filename == obj.x3dName+"_diff.png"):
						obj.matchedDiffTexName = obj.x3dName+"_diff.png"
						# print(potential_filename)
					elif(potential_filename == obj.x3dName+"_spec.png"):
						obj.matchedSpecTexName = obj.x3dName+"_spec.png"
						# print(potential_filename)
					elif(potential_filename == obj.x3dName+"_norm.png"):
						obj.matchedNormTexName = obj.x3dName+"_norm.png"
						# print(potential_filename)
					elif(potential_filename == obj.x3dName+"_emit.png"):
						obj.matchedEmitTexName = obj.x3dName+"_emit.png"
						# print(potential_filename)


	def writeToFile(self):
		UniqueObjectsList = []
		UniqueTexturesList = []

		for obj in self.objectList:
			if(obj.matchedName != "" and obj.matchedName not in UniqueObjectsList):
				UniqueObjectsList.append(obj.matchedName)

			if(obj.matchedDiffTexName != "" and obj.matchedDiffTexName not in UniqueTexturesList):
				UniqueTexturesList.append(obj.matchedDiffTexName)
			elif("default_diff.png" not in UniqueTexturesList):
				UniqueTexturesList.append("default_diff.png")

			if(obj.matchedSpecTexName != "" and obj.matchedSpecTexName not in UniqueTexturesList):
				UniqueTexturesList.append(obj.matchedSpecTexName)
			elif("default_spec_norm_emit.png" not in UniqueTexturesList):
				UniqueTexturesList.append("default_spec_norm_emit.png")

			if(obj.matchedNormTexName != "" and obj.matchedNormTexName not in UniqueTexturesList):
				UniqueTexturesList.append(obj.matchedNormTexName)
			elif("default_spec_norm_emit.png" not in UniqueTexturesList):
				UniqueTexturesList.append("default_spec_norm_emit.png")

			if(obj.matchedEmitTexName != "" and obj.matchedEmitTexName not in UniqueTexturesList):
				UniqueTexturesList.append(obj.matchedEmitTexName)
			elif("default_spec_norm_emit.png" not in UniqueTexturesList):
				UniqueTexturesList.append("default_spec_norm_emit.png")

		ofile = open(self.outputFileName, "w")

		ofile.write("# Converted .map file\n\n# Included Meshes (m)\n")

		for element in UniqueObjectsList:
			ofile.write("m %s\n" % element)

		ofile.write("\n# Included Textures (t)\n")

		for element in UniqueTexturesList:
			ofile.write("t %s\n" % element)

		ofile.write("\n# Object location, rotation, scale, and texture binding\n")

		for obj in self.objectList:
			ofile.write(obj.getDescriptor())

		ofile.close()


C = Converter()
C.convertFile(sys.argv)
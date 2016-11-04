import os
import sys

helpFileName = "x1_help.txt"
wikiFormatFileName = "x1_help_wikiFmt.txt"

helpFile = open(helpFileName, 'r')
outPutFile = open(wikiFormatFileName, 'w')

wikiHeader = "=="
wikiIndent = "::"

def formatColor(colorCount):
	if( (colorCount + 0xC0) <= 0xFF):
		return format( 0xD0D0C0 | (colorCount & 0xFF), 'x')
	else:
		#print "we are bigger than 0xff"
		return format( 0xD0D0C0 | ( (((colorCount & 0xFF) + 0xD0) << 2) & 0x00FF00), 'x')

def formatHeader(line, colorCount):
	temp = line.replace('\r', "") #remove carriage return
	temp = temp.replace(":", "")
	temp = temp.replace('\n', "") #remove newline
	temp = temp.strip()
	temp = wikiHeader + " " + temp + " " + wikiHeader + "\n" + \
	"<div style=\"background:#" + formatColor(colorCount) + ";padding:5px\">\n"
	return temp

def formatDescription(line):
	temp = line.split(':')[1]
	temp = temp.lstrip()
	temp = wikiIndent + "\'\'\'description:\'\'\' " + temp
	return temp

def formatArguments(line):
	temp = line.split(':')[1]
	temp = temp.lstrip()
	temp = wikiIndent + "\'\'\'args:\'\'\' " + temp
	return temp


def formatSubHeader(line):
	temp = line.replace("-", "") #remove dash preceding subcommands
	temp = temp.lstrip()
	temp = wikiIndent + temp

	return temp


#one should note. This script assumes that things are in the correct order.
#If they are not... this obviously won't work very well.
def main():
	countDirection = True # we start by incrementing
	colorCount = 0x0000
	firstHeader = True
	outPutFile.write("= Command Groups = \n")
	for line in helpFile.readlines():
		#if string contains "substring:" or is empty
		if( ("subcommands:" in line) or (not line.strip()) ):
			pass #do nothing

		elif(':' in line ): #it is either a header, description, or args
			#format and write the description
			if("description:" in line):
				outPutFile.write(formatDescription(line)) #write the description
			#Format and write the arguments
			elif("arguments:" in line):
				outPutFile.write(formatArguments(line)) #write the arguments
			#format and write the header
			else: 
				#close the div from the previous header
				if(firstHeader):
					pass
				else:
					outPutFile.write("</div> \n")
				#write out new header
				outPutFile.write(formatHeader(line, colorCount))
				firstHeader = False
				#Give the people something
				#print formatHeader(line).strip()

				#color changes for the wiki page. Because you gotta have color!
		

				#countDirection =True, then we are counting up. False, we are counting down
				if(countDirection and ( (colorCount + 0xD0C0) >= 0x00FFFF) ):
					countDirection != countDirection

				elif(not countDirection and ( (colorCount + 0xD0C0) <= 0x00D0C0) ):
					countDirection != countDirection

				
				if(countDirection):
					colorCount += 0x04
				else:
					colorCount -= 0x04


		#write out the commands under the group header
		else: 
			outPutFile.write(formatSubHeader(line))
		


if __name__ == '__main__':
	try:
		exit(main())
	except KeyboardInterrupt:
		sys.stderr.write('Cancelled by user\n')
#!/usr/bin/python

import os
import sys
import shutil

FileListBegin = '# FILE_LIST_BEGIN'
FileListEnd   = '# FILE_LIST_END'
NotFound        = -1
MultiOccurences = -2

def getCurrentPath():
    return os.path.dirname(os.path.realpath(__file__))

def getEverythingFilePath():
    return os.path.join(getCurrentPath(), 'ee-everything.cpp')

def getAndroidDotMkFilePath():
    return os.path.join(getCurrentPath(), 'Android.mk')

def getClassesDirPath():
    return os.path.join(getCurrentPath(), '..', '..', 'Classes')

def getSourceFilesInClassesDir():
    ret = []
    for file in os.listdir(getClassesDirPath()):
        if file.endswith('.cpp'):
            if ('everything' not in file) and ('Everything' not in file):
                ret.append(file)
    return ret

def updateEverythingFile():
    lines = []
    sourceFiles = getSourceFilesInClassesDir()
    for file in sourceFiles:        
        lines.append(r'#include "%s"' % file)
        lines.append('\n')
    f = open(getEverythingFilePath(), 'w')
    f.writelines(lines)
    f.close()

def updateAndroidDotMk():
    fin = open(getAndroidDotMkFilePath(), 'r')
    lines = fin.readlines()
    fin.close()
    beginLineIndex = NotFound
    endLineIndex = NotFound
    lineCount = len(lines)
    lineIndex = 0
    while lineIndex < lineCount:        
        if FileListBegin in lines[lineIndex]:
            if beginLineIndex == NotFound:
                beginLineIndex = lineIndex
            elif beginLineIndex != MultiOccurences:
                beginLineIndex = MultiOccurences
        if FileListEnd in lines[lineIndex]:
            if endLineIndex == NotFound:
                endLineIndex = lineIndex
            elif endLineIndex != MultiOccurences:
                endLineIndex = MultiOccurences
        lineIndex += 1
    if beginLineIndex == NotFound:
        print 'Not found FileListBegin'
        return
    if beginLineIndex == MultiOccurences:
        print 'Multiple occurrences of FileListBegin'
        return
    if endLineIndex == NotFound:
        print 'Not found FileListEnd'
        return
    if endLineIndex == MultiOccurences:
        print 'Multiple occurences of FileListEnd'
        return
    newLines = []
    lineIndex = 0
    while lineIndex <= beginLineIndex:
        newLines.append(lines[lineIndex])
        lineIndex += 1

    # Write new including files.
    newLines.append('\n')
    newLines.append('INCREMENT_BUILD := \\\n')
    sourceFiles = getSourceFilesInClassesDir()
    for file in sourceFiles: 
        newLines.append('../../Classes/%s' % file)
        if file != sourceFiles[len(sourceFiles) - 1]:
            newLines.append(' \\')
        newLines.append('\n')
    newLines.append('\n')

    lineIndex = endLineIndex
    while lineIndex < lineCount:
        newLines.append(lines[lineIndex])
        lineIndex += 1

    fout = open(getAndroidDotMkFilePath(), 'w')
    fout.writelines(newLines)
    fout.close()

updateEverythingFile()
updateAndroidDotMk()

lineCount = 0
files = os.listdir(getClassesDirPath())
for file in files:
    f = open(os.path.join(getClassesDirPath(), file))
    lineCount += len(f.readlines())
print 'loc = %d' % lineCount
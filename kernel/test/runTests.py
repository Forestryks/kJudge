#!/usr/bin/python3
import os
import subprocess
from sys import stderr, argv

class tcol:
	FAIL = '\033[31;1m'
	OK = '\033[92m'
	WARN = '\033[33;1m'
	END = '\033[0m'
	def col(s, c):
		return c + s + tcol.END

def buildAll(files):
	print('Building tests...')
	for file in files:
		cmd = 'cc {0}.c -o {0}'.format(file)
		print(cmd)
		try:
			if os.system(cmd) != 0:
				raise Exception
		except:
			print(tcol.FAIL + 'Compilation failed ({0}.c)'.format(file) + tcol.END)
			exit(-1)

	print('Build finished successfully')

def clearAll(files):
	for file in files:
		try:
			os.remove(file)
		except:
			print(tcol.WARN + 'Failed to remove {0}'.format(file) + tcol.END, file=stderr)

LEN = 12

class progress:
	LEN = 12
	cnt = -1
	
	def next(self):
		self.cnt += 1
		LEN = self.LEN
		cnt = self.cnt
		r = ' ' * LEN
		cnt %= (LEN - 3) * 2
		if cnt < LEN - 3:
			r = r[:cnt] + '.' * 3 + r[cnt + 3:]
		else:
			cnt -= LEN - 3
			r = r[:cnt] + '.' * 3 + r[cnt + 3:]
			r = r[::-1]
		r = '[' + r + ']'
		return r + ' ' + self.file

	def verdict(self, s):
		LEN = self.LEN
		pos = (LEN - len(s) + 1) // 2
		r = '[' + ' ' * pos + s + ' ' * (LEN - pos - len(s)) + ']'
		return r

	def passed(self):
		return tcol.col(self.verdict('PASSED'), tcol.OK) + ' ' + self.file

	def failed(self):
		return tcol.col(self.verdict('FAILED'), tcol.FAIL) + ' ' + self.file

	def __init__(self, file):
		self.file = file

def runAll(files):
	print('Running tests...')
	print('==================================================')

	all = len(files)
	passed = 0
	failed = 0
	# skipped = 0

	for file in files:
		bar = progress(file)
		print(bar.next(), end='')
		try:
			proc = subprocess.Popen('./' + file)
			while True:
				try:
					res = proc.wait(timeout=0.1)
					if res == 0:
						print('\r' + bar.passed())
						passed += 1
					else:
						print('\r' + bar.failed())
						failed += 1
					break
				except:
					print('\r' + bar.next(), end='')
		except:
			print('\r' + bar.failed())
			failed += 1
		del bar

	print('==================================================')
	print('Summary:')
	print(tcol.col('Passed: {0}'.format(passed), tcol.OK))
	print(tcol.col('Failed: {0}'.format(failed), tcol.FAIL))

def isTest(file):
	a = file.split('.')
	return len(a) >= 3 and a[-2] == 'test'

def runTests():
	files = [file[:-2] for file in os.listdir('.') if isTest(file)]

	buildAll(files)
	runAll(files)
	clearAll(files)

if __name__ == "__main__":
	runTests()

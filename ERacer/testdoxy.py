"""@package This is module documentation
blah blah"""
class doxy:
	"""This is a test class"""
	
	def __init__(self, val):
		"""Constructor"""
		self.value = val
		
	def add(self, a):
		"""This function adds a number to value"""
		self.value += a

def add(a, b):
	"""This is documentation for a function outside of the class"""
	return a+b

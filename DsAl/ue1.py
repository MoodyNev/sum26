array = [1,2,10,51,55,99, 100, 105]
number = 51
def numberInArray(array, number):
	loops = 0
	firstNumber = array[0]
	lastNumber = array[len(array) - 1]
	if(number < firstNumber): return False
	if(number > lastNumber): return False
	middle = len(array) // 2
	if(number < array[middle]):
		for i in range(0, middle):
			loops += 1
			if(number == array[i]): 
				print(loops)
				return True
	
	else:
		for i in range(middle, len(array)):
			loops += 1
			if(number == array[i]):
				print(loops)
				return True

	print(loops)		
	return False

print(len(array))
print(numberInArray(array, number))

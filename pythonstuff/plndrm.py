for i in range(int(input())):
    word =  input()
    newWord = []
    for k in range(len(word) - 1, -1, -1):
        newWord.append(word[k])
    if(word == ''.join(newWord)):
        print("Palindrom")
    else:
        print("False")
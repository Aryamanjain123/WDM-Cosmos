text1="Last night I stayed up until three o'clock working on a presentation for my summer class. After I fi"
text2="My favorite color has got to be green. I heard that looking at it calms down your body and soothes y"
text3="I love frying eggs. But I don't like to eat the fried eggs that I cook. Whenever I try to offer them"
stringy = ""
for i in range(len(text1)):
    stringy += text1[i]
    stringy += text2[i]
    stringy += text3[i]
    
print(stringy)
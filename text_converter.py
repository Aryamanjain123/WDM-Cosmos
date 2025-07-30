text1="Last night I stayed up until three o'clock working on a presentation for my summer class. After I finished that, it was late so I tried to go to sleep. I could not."
text2="Our favorite color has got to be green. We heard that looking at it calms down your body and soothes your soul. I held this belief until I got lost in a rainforest."
text3="I love frying eggs. But, I don't like to eat the fried eggs that I cook. Whenever I try to offer them to my family, I'm always met with rejection. I will never eat."
stringy = ""
for i in range(len(text1)):
    stringy += text1[i]
    stringy += text2[i]
    stringy += text3[i]
    
print(stringy)
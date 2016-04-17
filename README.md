# Neural-Cocktail-Party-Processor
A tiny neural network that decouples overlapping frequency forms aka sounds

This project took place 1994 an was part of something that is comparable to a bachelor thesis in psychology today.
The implementation is based on the article: 
Von der Malsburg, C., & Schneider, W. (1986). A neural cocktail-party processor. Biological cybernetics, 54(1), 29-40.

It is intended to understand a phenomenon gestalt psychologists discovered. Imagine you are in a cocktail party and dozends of people
around you talk ceasinglessly. Effortlessly you can follow the conversation with your chat partner. Gestalt psychologists said that 
your mind can follow the form (Gestalt) of the voice pattern of your partner easily.

This model takes it on a much simpler level and works with sounds. One sound is introduced and then overlapping sounds are introduced.
The implementation can seperate different sounds.

All is very primitve though, you have to feed the 'sounds' as an ascii text file of excitiation patterns for the different neurons. 
These excitiation patterns correlate with what we know about the human auditory system. Research "Neuronal encoding of sound", there is
also a good wiki about that: 
https://en.wikipedia.org/wiki/Neuronal_encoding_of_sound

Since this project was implemeted in 1994 on a Lattice C++ compiler it will not run.
However I hope that some of you might find the ideas helpful!

Regensburg, Germany, 2016-04-17

Code on!
Ferenc

PS: If someone feels inclined to translate the German content into English you have my eternal gratitude!

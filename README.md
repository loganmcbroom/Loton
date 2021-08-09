# Loton
![Loton Logo]( https://github.com/loganmcbroom/Loton/blob/main/Resources/Media/app-icon.ico )

Loton is a near-real-time experimental audio processing workstation focused on functional control over process parameters, transforms 
that couldn't be achieved in real-time, and creative use of the phase vocoder transform.
It's named after James Loton Flanagan, one of the creators of the phase vocoder transform.

# Building

### Dependencies:

#### Qwt:
I don't know of a build of this not relying on QtCreator.  
Download qwt-6.1.6 from here: https://qwt.sourceforge.io/  
Unzip it  
Open qwt.pro in QtCreator  
Hit build  

#### LuaJit:
```
git clone https://luajit.org/git/luajit.git
cd luajit
make
```
Make sure the luajit folder is in the system PATH.

#### Flan:
See https://github.com/loganmcbroom/flan

#### My NodeEditor fork:
See https://github.com/loganmcbroom/nodeeditor

### Loton:
```
git clone https://github.com/loganmcbroom/Loton
cd Loton
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
Finally, make sure the qwt and lua shared libraries are visible.

# Usage

### Program Layout
Loton has five panels. On the left from top to bottom are the file explorer, the process explorer, and the node view.
On the right is the flow view, and below it, the help panel. The edges of these panels can be dragged to resize them.
The file explorer handles loading and saving audio and pvoc files in from the computer.
The process explorer contains every type of node available in Loton. It has a search bar above it that can be accessed with ctrl+f.
The node view appears when a node in the flow view is selected, and provides finer controls over node parameters, and occasionally additional parameters.
The flow view is where nodes live and where processing happens.
When a node is selected, the help panel contains info on what the node does, and what parameters it takes.


### Basic usage
Before getting started making sounds, go to File -> Settings, and set the samples and projects folders.
The sample folder will now be shown in the file explorer, and samples can be dragged into the flow view to create Audio Source nodes.
Note that mp3 files are not currently supported.
To do anything with the Source node, we need to create a process node. This can be done by either dragging a process name from the process explorer into the flow view, or 
right clicking the flow view and selecting a process name there. Try dragging an Audio Repitch into the flow view.
Nodes have input and output ports on the left and right respectively. Dragging the output port of the Audio Source to the top input of the Audio Repitch will create a Connection. 
Note that a single output port can be connected to any number of input ports.
In the repitch node there is a number slider. These can be dragged to set the number, but you can also double click them to type the input, or ctrl-click them to reset them to the default.
When you change the slider, the repitch node will reprocess the Audio Source handed to it. In general, nodes will always recompute the process they represent when given new data.
Once the process completes (this should be instantaneous for repitching), the play button can be pressed to hear the result.

This is the basic workflow of Loton. Connecting nodes in sequence and modifying parameters, and listening to the outputs until you like what you hear.
When you are ready to save a sound you've made, either drag the waveform icon back into the file explorer, or click the file button on the node.
To learn more about any process, create a node for it and examine the help panel.


### Function Nodes
You've likely noticed that the Repitch node has a second input. 
Most node parameters have constant inputs that can be set using number sliders in the node, but these can be overriden with functional inputs.
Functional inputs allow changing process parameters over time, and as we will see later, over frequency and index.
In the process explorer, open the Functional Nodes and drag a 1->1 Polynomial into the flow view. The four number sliders represent the coefficients of a cubic equation.
Set the first and second parameters to 1, and then connect the output of the Polynomial to the second input of the Repitch. After checking the output of the repitch, try
modifying the Polynomial further. Be aware that the polynomial passing through zero during the Audio time domain may take some time to process, and will produce long outputs.

Function nodes come in three varieties, 1->1, 2->1, and 2->2. These almost always represent functions of time in the case of 1->1s, and functions of time and frequency in the other cases.
The individual nodes explain what the function outputs will represent in the help panel. For information on function inputs, see the section on feedback nodes.

Distribution nodes work differently from other function nodes. The graph they display is not the output of the function, but instead a probability density function.


### The Phase Vocoder
The phase vocoder is a short-time Forier transform that utilizes phase information over time to give a reasonable estimate of frequency information in the input audio over time.
In Loton, the output of this transform is called PVOC data. In the same way that audio data is a one dimensional grid of samples, PVOC data is a two dimensional grid of samples.
The x-axis represents time, and the y-axis frequency. For audio data, each sample is a measurement of pressure at the given time. 
For PVOC data, each sample contains both the magnitude and frequency of a harmonic at a given time and frequency.
There is some confusion to be had about measuring frequencies at a given frequency. When we measure at a given frequency, we are looking for harmonics that are close to the point at which we are measuring,
but the recorded PVOC sample will contain a frequency slightly off from the point of measure.

PVOC data is usually used for pitch-invariant time stretching, which can of course be done in Loton, but there are a great many other transforms available for manipulating PVOC data.
These manipulations are one of the key features of Loton. Using a PVOC process is easy thanks to automatic conversions. Throughout Loton, any time a data type can be transformed into another data type,
That transform will be applied automatically by connections. To see an example of this, try connecting an Audio Source to a PVOC Resonate. Note the symbol on the connection, and the change in color.
This shows that the audio has been automatically conveted to PVOC, before being sent to Resonate. Note that the play button on the PVOC nodes may take some time to start playing,
as they have to convert the PVOC back to audio internally before playback can begin. 

The phase vocoder has some parameters that can change how other transforms sound. These can be modified globally in the
settings menu, or you can use an explicit Audio Convet To PVOC node, and its PVOC analog.


### Feedback Nodes
There are some processes that use other processes as they are applied. These are called Feedback Nodes. Audio Iterate is the most simple of these, taking an audio and repeating it a given number of times.
Using feedback, each iteration can be processed individually. For each iteration, the FB output of Iterate will send the input to a processing chain that can apply any number of transforms, before sending
the processed audio back into the Feedback input of the Iterate node. Try connecting an Audio Repitch to the feedback inputs and outputs. 
This will, as expected, repitch each iteration to whatever the repitch node is set to, which isn't very interesting. In order to do something interesting, we need to use the FB Time output to modify
how the Repitch changes over time. For each iteration, FB Time will output the time at which the iteration will sit in the output.
Connect the FB Time output to the input of a 1->1 Polynomial, and connect that to the Repitch Factor. Function nodes have a Number input, which when connected will make the function node return its 
function evaluated at the input. Using polynomial coefficients 1, 1, 0, 0 will now repitch the iterated audio based on the time of the iteration.

Feedback nodes always have a "Recursion" setting, which will send the previously processed audio from the FB output rather than the original audio.
Distribution node inputs don't apply a function to the input, but instead use the input as a fixed seed.


# User manual

## The Node
A *Node* is a bock thats has inputs and outputs. These inputs or outputs are called *Interfaces*. 
You can connect *Interfaces* via a *Cable*.
A *Cable* routes an input *Interface* to an output *Interface*. It is a visual representation of the connected *Interfaces*.
*Nodes* can be routed and configured via the *Node Editor*. 

** PICTURE OF NODE IN NODEEDITOR **

### Controlling vs. Configuring vs. Programming
Normally these two words are near togehter, linguistically speaking. But in Skui they describe to fundamental differenct things.
When you *Control* Skui, you run it and change things by controlling *Panels*. Each *Panel* has *Visuals* where the controlling happens. A *Visual* could be a Nob or a Slider.
But also Graphs and Progess Bars are *Visuals*. Controling only works in *Run Mode*.

If you are in *Edit Mode* you can't *Control*, as said above. At the *Panel* you can now *Configure* *Visuals*. You can change size or move a *Visual*. 
By right clicking you can change properties of the *Visual*. This could be a range for a slider or the color of a button.
But you can not only *Configure* *Visuals*. You can *Configure* every *Node* using the *Node Editor*. 
You might want to change the Baud Rate *Property* of a Serial *Driver* or the location of a *Python Script*.
The *Configuration* is the initial state in that the program starts with. But what if I want change behaviour at runtime and implement logic?

*Programming* is the right thing to do. It happens in the *Node Editor* where you *Connect* different *Nodes* to each other.
By this you create *Logic Paths* from one *Node* to another. One of the easies exampeles for this is the connection between a Slider and a Progress Bar.
First you create two *Visual* *Nodes*. One Slider and One Progress Bar. By conecting the *Output* of the Slider *Node* and the *Input* of the Progress Bar *Node* you get a *Connection*.
Every time you *Control* or in more non Skui words change the Slider, so will the Progress Bar.

### The Panel
The *Panel* is the main panel to control. It is not used for linking your Nodes. You can use the *Node Editor* for that.
The *Panel* contains *Visuals* 

#### Working with Edit Mode
Edit mode is the mode to change your *Visuals* at the *Panel*. You can configure them as you like.

You can select an *Viusal* by clicking on it. A bounding box pop's up. It is now possible to drag 
or resize the *Visual*.

If you want to select multible *Visuals* you can either press the mouse and drag or press shift 
and click on the *Visuals* you want to select.

**GIF ANIMATION**

### The Node Editor
You might have defined and set up some *Visuals* at the *Panel*. But there is a big problem ...
they are dead inside, have no soul or to use more rational terminology, they lack logic.
To define logic, you have the *Node Editor*. You can *Connect* different *Nodes*.

#### The behaviour of the Editor
You can insert *Nodes* by right clicking and selection the *Node* you want to insert.
It will spawn at the positon where you right clicked. 
If you are right clicking while you are *Routing* a *Cable*. 
The spawned *Node* will be automaticlly connected to the active *Cable*.

By left clicking on a *Pad* a new *Cable* be created. *Cables* can only go vertical or horizontal.
If you have your *Cable* active after initiating it at a *Pad*. You can left click again to switch direction.

By a left click on a *Node* you can select the Node, hold Shift to select more. 
You can hold left to move the selected *Node*/*Nodes* or press Delete to delete them.




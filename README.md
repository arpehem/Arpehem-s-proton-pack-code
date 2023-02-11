# Arpehem-s-proton-pack-code
This is the code to use if you realyzed you bought a **common cathode bargraph** or only have access to this model.
This code is designed to be used with **2 Arduino nanos** or your favorite boards that have at least as many i/o ports and handles Arduino in order to minimyze the amount of wires running from the wand to the pack and has a unique smoke purge seuqence and design as well as all the bargraph animations.

Check out this link to see it in action:
https://youtu.be/VGt3vFmbnvs

Features:
- Common cathode bargraph full animations
- aRGB 4x5mm aRGB wand tip LED lights
- Functional top and front wand knobs
- Music mode and volume control on the wand
- Bargraph volume display
- Full pack aRGB controled
- New N-filter realistic smoke purge system
- DYI smoker build
- Compatible with pre-built smoker
- Uses DFplayer mini
- Customisable sound amplification


Upcomming updates:
- Firing tail sound 
- Game modes
- Top knob functionality

Which code to use?
- Use Neutrino wand code for the nano placed in the wand.
- Use Cyclotron_single_lights on your pack nano if you have single aRGB cyclotron lights.
- Use Cyclotron if you have 7 LED aRGB pucks for your cyclotron. Know that this code may be outdated, only single lights version is being worked on trough the end of this project as it was a design decision change but I wanted to include the option to use what you have/want and update the code yourself. (I was almost done I beleive and it is functional)

How it works:
There is a sequence to turn the pack on that has to be respected. First step is to activate the particule accelerator controller (wand) on, flip on the wand light switch. That turns on the wand top light on indicating it's under tension. Second we flip the boot switch on which boots the pack and the rest of the wand. The bargraph lights up by doing it's boot animation, then falls into it's loop at the same time as the pack starts and the 2 white hat lights on the wand also start flashing. The third switch is to turn the smoke effect on and off as it is not allowed in all conventions.

Firing is fairly straight forward, just press the fire pin button. When firing, the 4 wand tip lights are controlled by a random function made to have white flashes, most of the time orange lights with hints of blues. The orange hat light also lights up when firing. At any time while firing, pressing the vent pin button will call a vent function that will pause the flashing light animation on the wand whyle the cyclotron is purging. If you installed a smoker, when you vent before entering the overhead sequence, there will be a breif lag (half a second) in between the command and the action to allow the smoker to build up a bit of smoke for a better effect. Shooting will make the cyclotron go faster untill it reaches the ovearheting sequence.

If you shoot long enough, you will enter the overheating sequence that will change the bargraph animation, sound the allarm, make the 3rd wand top light flash, flash all 4 cyclotron lights at the same time and alter LEDs on the Power cell. If you let go of the firing button or if you press the vent button in that 'mode', it will engage the vent sequence that will light up the N-Filter, activate the purge fan propelling the smoke out of the N-filter and return to normal cycle loop afterwards.

If you keep shooting, the pack will force vent, doing all the thing normal venting would do but insttead of going back to the normal cycle sequence, it will force shutdown. This is by design. If this happens to you, you have to turn off both the boot and the wand pin switches and switch them back on in the propper order for it to restart. Only shutting down the pack switch on the wand will not allow to reboot, the user has to complete the shutdown sequence by turning everything off before turning it back on.

Controlling the music and the volume is done with the front knob which is attached to the rotary encoder. Pressing it once will put it in volume mode which will display it's value on the bargraph. Raisin the volume will raise the bargraph value and vice versa. Clicking it once more will bring it back to normal mode. Double clicking the rotary encoder will start the music and holding it down will skipp to the next song. Clicking it twice again will bring it back in normal pack mode.

Required components:
-  Audio option 1 (speakers and sub watts have to be a bit more than the chosen amp output to avoid any problems):
- - 1 x Subwoofer the size that fits your motherboard speaker hole.
- - 2 x 3" car speaker 
- - 1 x amp with 2 speaker channels and 1 sub channel
- Audio option 2 (speaker wattage has to be a bit more than the chosen amp output to avoid any problems):
- - 1 x Speaker that fits your motherboard speaker hole.
DIY Smoker:
- 1 x 510 to EGO adapter (optional but way easier to use)
- 1 x 12a 100W buck converter
- 2 X 20a relay
- 1 x 0.8 omh Vape tank
- 1 x 50mm 230mah blower fan
- 3d printer
- 3" of 1/4" plastic tubing that can be found in hardware stores

Wand and pack:
- 1 x 2 way rotary switch (https://amzn.to/40L4XQX)
- 4 x 41x33mm Aluminum LED Flashlight Reflector Condenser Cup Cap
- 4 x 8mm aRGB LED (5mm could fit too)
- 1 x 6000 Mah battery pack (Recommend Talentcell)
- 3 x 20a relay
- 1 x 5V 40mm fan
- 2 x 3a buck converter
- 1 x 360 Degree Rotary Encoder Module
- 2 x Arduino nano (Or other boards with as many I/O ports that handles Arduino)
- 1 x Adafruit 16x8 LED Matrix Driver Backpack - HT16K33 Breakout
- 1 x Common cathode 28 segment LED bargraph
- 2 x Adafruit NeoPixel Stick for Arduino- 8 x WS2812
- 4 x 5mm aRGB LED
- 2 x 7mm Momentary (OFF-ON) Push Button Micro Switch
- 3 x 2 Position 3 Pins Mini Toggle Switch
- 1 x SS-10-23NP-L-E Rotary switch (https://bit.ly/3HUpoSU)
- 3 x 5mm white LED
- 1 x 3mm red LED
- 1 x 5mm yellow LED
- 1 x 5V white LED strip (about a foot)

Pinout:
Wand: 
- D2 Boot pin (wand switch 2)
- D3 Light switch (wand switch 1)
- D4 Smoke switch (wand switch 3)
- D5 Fire pin (fire button)
- D6 Vent pin (vent button)
- D7 Btn pin (Rotary encoder btn pin)
- D8 DT  (Rotary encoder data pin)
- D9 CLK  (Rotary encoder clock pin)
- D10 Cannon LED aRGB (Wand tip LED)
- D11 Fire light (wand tip orange light)
- D12
- D13 Rotary switch 1 (top knob rotary switch output 1)
- A0 Slo-blo LED (wand red light)
- A1 Wand white flashing hat light 1 (Wand top light large)
- A2 Wand white neon hat light (Wand top light small)
- A3 Wand white flashing hat light 2 (Wand top light large)
- A4 HT16K33 matrix SDA
- A5 HT16K33 matrix SCL
- A6 Rotary switch 2 (top knob rotary switch output 2)
- A7 Rotary switch 3 (top knob rotary switch output 3)

Pack:
- D2 Cyclotron aRGB data
- D3 Powercell aRGB data
- D4 Purge pin (Purge relay)
- D5 Smoke pin (smoke relay)
- D10 DFplayer mini RX
- D11 DFplayer mini TX


Instructions:
- Start by running power from the main power switch (2 way rotary switch) connected to a buck converter that lowers the 12V to 5V to the 2 arduinos separately. Do not use 5V directly from the battery pack, it causes issues. Connect the first arduino's RX to the second one's TX and the first one's TX to the second one's RX. Basically, connect them backwards to one another. Now the wand 'speaks' to the pack.
Follow these special steps and then connect everything else that is iddentified in the pinout section.
- Wand tip: 
Daisy chain 4 5mm LEDs and connect the first one's data input to wand D10.
- Power cell:
Sawder the output contacts of the first bar onto the input contacts of the second one to make a 16 bit led bar. Connect the first bar data input to pack D3.
- Cyclotron:
Glue in the 8mm led INSIDE the flashlight reflector or trough them if you got 5mm aRGB LEDs instead. Connect the first one's data pin to pack D2.
- Smoker:
We're going to push air in the vape's air inlet instead of pulling it out. Use a dremel to marry the blower fan to the side of the vape tank. Use a tie-wrap to secure the blower fan onto the base of the vape tank alligned with the vape tank's air intake. Dril a hole in your 510 to EGO adapter's base and use it to sawder in a wire to ground. Sawder directly onto the midle contact for the positive contact. Screw in the vape tank adaptor and connect it to the high power buck converter. Connect the high power buck converter to a 20a relay. That relay is controlled by the smoke pin Arduino output. The white 5v LED strip is to be connected to it's own relay and so does the 40mm purge fan. Theire individual relays are activated by a third relay that is driven by the smoke purge pin.
You will need to print these files for the purge tank:
https://www.thingiverse.com/thing:5848057
You'll need these pieces:
- 45_intake_2
- 90_angle
- Vent_cup
- 90_intake_2
- and a tip diffuser of your liking listed in the files.
You have an array of tips to choose, I prefer the 1 row 1 tip diffuser.
Glue on the tip on the straight end of the 90_intake_2 piece. Glue the 45_intake_2 in the side hole of the vent_cup. Glue to 90_intake_2 to the end of the 45_intake_2 end in the vent_cup. Use 1/4" plastic tubing to connect the intake to the 90_angle and then to then output of the vape tank.
The parts are jointed by 1/4" plastic tubing.
- Audio:
Connect the speaker(s) to the amp. Power the amp straight from the powercell. Don't use power converters as it will induce interference in the audio signal. Connect the amp's ground the main power switch (or power if you decided to control power by the positive). Connect the DFplayer mini outputs to the amp's input. Unpack the MP3.rar in a folder called MP3 on a SD card and insert it in de DFplayer mini.

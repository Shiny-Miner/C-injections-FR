To support this types of codes and coders join our discord https://discord.com/invite/axNX3rhR9K
# Continue screen-BW
A C injection for inserting a new  continue screen from Pokemon B/W to BPRE(firered). This code is taken from a pokefirered rom hack.
#Screenshot:

## Credits : Ansh, Shiny-Miner, Compumaxx
***Notes:*** 
- DevkitARM and ARMIPS are required. - To compile it is necessary to have preproc.exe and gbagfx.exe within some path of the PATH variable
- Open the config.mk file, find and change fa0000 of the following line to an offset aligned with enough free space: `INSERT_INTO ?= 0x08fa0000
- Name your rom/rom to insert the code BPRE0.gba and put it in the folder
- Compile by running make with your terminal, and a rom with the injection will appear in a folder called `build`.
- Files inside the `include` folder were taken from [**pokefirered**](https://github.com/pret/pokefirered)..


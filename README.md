porting summary screen gen V
https://github.com/CompuMaxx/TerranovaFR/commit/4961273f5c1e20e8bd2a760ced929aaeb7d0b573
# Pokemon GBA hack template
 Plantilla para realizar hacks en C en juegos de pokémon GBA.


***Notas:***

- DevkitARM y ARMIPS son necesarios.

- Para compilar es necesario tener preproc.exe y gbagfx.exe dentro alguna ruta de la variable PATH

- Abrir el archivo config.mk, buscar y cambiar fa0000 de la siguiente línea por un offset alineado con suficiente espacio libre:
        `INSERT_INTO ?= 0x08fa0000`
- En el archivo config.mk, buscar la siguiente línea
        `ROM_CODE ?= BPRE`
    - mantener  BPRE para compilar usando Fire Red
    - cambiar a BPRS para compilar usando Rojo Fuego en español
    - cambiar a BPEE para compilar usando Emerald

- Compilan ejecutando make con su terminal, y una rom con la inyección aparecerá en una carpeta llamada `build`.

- Pueden usar en un script `callasm` seguido por el offset+1 donde insertaron el código, para llamar la rutina.

- Archivos dentro de la carpeta `include` fueron tomados de [**pokefirered**](https://github.com/pret/pokefirered).


# denki-kurage/enclosure

## Prerequisites

- OpenSCAD (nightly build) 2026.01
- colorscad (1c5a06bd)
- GNU Make
- ImageMagick
- BOSL2 (included as submodule)

## Build

```sh
# Sync submodules
$ git submodule update --init --recursive

# Generate .scad and .stl files.
$ make all -B

# Generate thumbnail images.
$ make images -B

# Generate the .3mf file.
$ OPENSCAD_CMD=openscad-nightly colorscad -i dk_stand.scad -o dk_stand.3mf -f
```

## Parts

|                                   | STL (Single color)             | 3MF (Multi-color)              |
| --------------------------------- | ------------------------------ | ------------------------------ |
| ![](./images/dk_stand_bottom.jpg) | [dk_stand.stl](./dk_stand.stl) | [dk_stand.3mf](./dk_stand.3mf) |

## Print conditions

- Multi color printing (Optional, Use .3mf)
- 1.75mm PLA (Transparent, Matte Blue)
- 0.12mm layer height

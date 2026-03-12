# <img src="./assets/denki-kurage.png" height="32px"> Denki Kurage

`Denki Kurage` (ja: 電気くらげ, electric jellyfish)

A sort of an artificial jellyfish on CYD (Cheap Yellow Display, ESP32).

## BOM

| Component      | Quantity | Notes                           |
| :------------- | :------- | :------------------------------ |
| ESP32-2432S028 | 1        | 2.8" TFT "Cheap Yellow Display" |

## 3D Printed Parts

For .stl and .scad files, see [`./enclosure`](./enclosure).

<img src="./enclosure/images/dk_stand_bottom.jpg" height="150px">

## Controls (Touch)

| Area                                 | Action             |
| :----------------------------------- | :----------------- |
| Top / Bottom strip (Left side)       | **Move Up / Down** |
| Middle left / Middle right           | **Rotate Camera**  |
| Center area                          | **Cycle Colors**   |
| Top-right corner (Solid / Wireframe) | **Toggle Mode**    |
| Bottom-right corner                  | **Debug Info**     |

## Build

Built and flashed using [PlatformIO](https://platformio.org/).

```bash
pio run -t upload
```

## Gallery

<div>
<img src="./assets/denki_kurage_front_3.jpg" height="300px">
<img src="./assets/denki_kurage_side.jpg" height="300px">
<img src="./assets/denki_kurage_back.jpg" height="300px">
<video src="" height="300px" autoplay loop muted playsinline></video>
</div>

## Acknowledgements

- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) by Bodmer
- [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen) by Paul Stoffregen
- [ESP32-Cheap-Yellow-Display](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display) by Witnessmenow
- [BOSL2](https://github.com/BelfrySCAD/BOSL2) library for enclosure design
- [M+ FONTS](https://mplusfonts.github.io/) for typography
- [人工くらげ](https://jnc-klg.sakura.ne.jp/) for the inspiration
- A reference to Philip K. Dick's [Electric Sheep](https://en.wikipedia.org/wiki/Do_Androids_Dream_of_Electric_Sheep%3F) (ja: 電気羊)

## License

- **Software**: MIT
- **Font (M+ FONTS)**: [SIL Open Font License](https://scripts.sil.org/OFL). M+ FONTS are free to use, modify, and redistribute.

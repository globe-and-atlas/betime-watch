"""Build ordered store media directly from the verified native capture matrix."""
from pathlib import Path
from shutil import copyfile

from PIL import Image

MEDIA = Path(__file__).resolve().parents[1] / "media"
PALETTES = ["default", *(f"palette-{i}" for i in range(1, 6)), "custom"]
STILLS = ["default", "palette-2", "palette-3", "clay-save"]


def main():
    output = MEDIA / "store"
    output.mkdir(exist_ok=True)
    for platform, size in [("emery", (200, 228)), ("gabbro", (260, 260))]:
        frames = []
        for palette in PALETTES:
            with Image.open(MEDIA / f"{platform}-{palette}.png") as source:
                assert source.size == size
                frames.append(source.convert("RGB"))

        animation = output / f"{platform}_01_motion-colors.gif"
        frames[0].save(
            animation, save_all=True, append_images=frames[1:],
            duration=[1800] + [1350] * 6, loop=0, disposal=2, optimize=False,
        )
        with Image.open(animation) as decoded:
            assert decoded.n_frames == len(frames)
            for index, expected in enumerate(frames):
                decoded.seek(index)
                assert decoded.convert("RGB").tobytes() == expected.tobytes()

        for index, still in enumerate(STILLS, start=2):
            copyfile(MEDIA / f"{platform}-{still}.png",
                     output / f"{platform}_{index:02d}_{still}.png")

    with Image.open(MEDIA / "gabbro-default.png") as source:
        for name, size in [("small", 80), ("large", 144)]:
            source.resize((size, size), Image.Resampling.LANCZOS).save(
                output / f"icon-{name}.png"
            )


if __name__ == "__main__":
    main()

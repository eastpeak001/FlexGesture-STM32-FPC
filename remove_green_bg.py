from __future__ import print_function

import argparse
from pathlib import Path

from PIL import Image, ImageFilter


DEFAULT_SRC = Path(r"D:\动画素材\正常待机")
DEFAULT_OUT = Path(r"D:\动画素材\正常待机_透明背景")


def clamp(value, low, high):
    return max(low, min(high, value))


def remove_green(path, out_path, edge_contract=1, blur_radius=0.65):
    image = Image.open(str(path)).convert("RGBA")
    width, height = image.size
    pixels = image.load()

    green_candidate = bytearray(width * height)
    for y in range(height):
        for x in range(width):
            r, g, b, _ = pixels[x, y]
            max_rb = max(r, b)
            min_rb = min(r, b)
            green_advantage = g - max_rb
            green_balance = g - min_rb

            # Key only the green screen color family; red hair and eye colors stay opaque.
            if g > 40 and green_advantage > 6 and green_balance > 12:
                green_candidate[y * width + x] = 1

    # Use all keyed pixels, not only pixels connected to the image edge. Some frames
    # have green-screen pockets visually enclosed by hair, sleeves, and the skirt.
    mask = Image.frombytes("L", (width, height), bytes(v * 255 for v in green_candidate))
    for _ in range(edge_contract):
        mask = mask.filter(ImageFilter.MaxFilter(3))
    mask = mask.filter(ImageFilter.GaussianBlur(blur_radius))
    mask_pixels = mask.load()

    for y in range(height):
        for x in range(width):
            r, g, b, a = pixels[x, y]
            transparent = mask_pixels[x, y] / 255.0
            new_alpha = int(round(a * (1.0 - transparent)))

            max_rb = max(r, b)
            if g > max_rb:
                spill = max(transparent, clamp((g - max_rb) / 90.0, 0.0, 1.0)) * 0.9
                g = int(round(g * (1.0 - spill) + max_rb * spill))

            if new_alpha <= 2:
                pixels[x, y] = (0, 0, 0, 0)
            else:
                pixels[x, y] = (r, g, b, new_alpha)

    out_path.parent.mkdir(parents=True, exist_ok=True)
    image.save(str(out_path))


def validate_alpha(path):
    image = Image.open(str(path)).convert("RGBA")
    width, height = image.size
    alpha = image.getchannel("A")
    corners = [
        alpha.getpixel((0, 0)),
        alpha.getpixel((width - 1, 0)),
        alpha.getpixel((0, height - 1)),
        alpha.getpixel((width - 1, height - 1)),
    ]
    transparent = sum(1 for value in alpha.getdata() if value == 0)
    opaque = sum(1 for value in alpha.getdata() if value >= 250)
    return max(corners) == 0 and transparent > 0 and opaque > 0


def main():
    parser = argparse.ArgumentParser(description="Remove green-screen backgrounds from PNG frames.")
    parser.add_argument("--src", default=str(DEFAULT_SRC), help="Input folder containing PNG frames.")
    parser.add_argument("--out", default=str(DEFAULT_OUT), help="Output folder for transparent PNG frames.")
    parser.add_argument("--edge-contract", type=int, default=1, help="Expand keyed area slightly to remove green fringe.")
    parser.add_argument("--blur-radius", type=float, default=0.65, help="Soft alpha edge radius.")
    args = parser.parse_args()

    src = Path(args.src)
    out = Path(args.out)
    files = sorted(src.glob("*.png"))
    if not files:
        raise SystemExit("No PNG files found in: {}".format(src))

    out.mkdir(parents=True, exist_ok=True)
    failures = []
    for path in files:
        target = out / path.name
        remove_green(path, target, args.edge_contract, args.blur_radius)
        if not validate_alpha(target):
            failures.append(target.name)

    print("processed={}".format(len(files)))
    print("output={}".format(out))
    if failures:
        print("validation_failed={}".format(",".join(failures)))
        raise SystemExit(2)
    print("validation=ok")


if __name__ == "__main__":
    main()

from pathlib import Path
from textwrap import wrap

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parent
OUT = ROOT / "SCAPMS_FINAL_SUBMISSION.pdf"
FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf"
if not Path(FONT_PATH).exists():
    FONT_PATH = "/System/Library/Fonts/Helvetica.ttc"


def font(size, bold=False):
    try:
        return ImageFont.truetype(FONT_PATH, size, index=1 if bold else 0)
    except Exception:
        return ImageFont.load_default()


def text_pages(path, title):
    text = path.read_text(encoding="utf-8")
    pages = []
    lines = []
    for raw in text.splitlines():
        if not raw:
            lines.append("")
            continue
        if raw.startswith("#"):
            raw = raw.lstrip("# ").upper()
        lines.extend(wrap(raw, width=92) or [""])
    for start in range(0, len(lines), 43):
        image = Image.new("RGB", (1275, 1650), "white")
        draw = ImageDraw.Draw(image)
        draw.text((70, 55), title, fill="#17324d", font=font(30, True))
        y = 120
        for line in lines[start:start + 43]:
            draw.text((70, y), line, fill="#17212b", font=font(20))
            y += 33
        pages.append(image)
    return pages


def diagram_page():
    source = ROOT / "rendered" / "UML_CLASS_DIAGRAM.svg.png"
    image = Image.open(source).convert("RGB")
    image.thumbnail((1100, 1300))
    page = Image.new("RGB", (1275, 1650), "white")
    draw = ImageDraw.Draw(page)
    draw.text((70, 55), "UML / CLASS DIAGRAM", fill="#17324d", font=font(30, True))
    page.paste(image, ((1275 - image.width) // 2, 150))
    return page


def source_pages():
    project_root = ROOT.parent
    files = [project_root / "CMakeLists.txt"]
    files += sorted((project_root / "include").rglob("*"))
    files += sorted((project_root / "src").rglob("*"))
    files += sorted((project_root / "data").rglob("*"))
    files += sorted((project_root / "tests").rglob("*"))
    pages = []
    for path in files:
        if not path.is_file():
            continue
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        for start in range(0, max(1, len(lines)), 58):
            image = Image.new("RGB", (1275, 1650), "white")
            draw = ImageDraw.Draw(image)
            draw.text((55, 35), f"SOURCE CODE: {path.relative_to(project_root)}",
                      fill="#17324d", font=font(24, True))
            y = 85
            for number, line in enumerate(lines[start:start + 58], start + 1):
                rendered = line[:118]
                draw.text((55, y), f"{number:4} {rendered}", fill="#17212b",
                          font=font(15))
                y += 26
            pages.append(image)
    return pages


pages = text_pages(ROOT / "PROJECT_REPORT.md", "PROJECT REPORT")
pages.append(diagram_page())
pages.extend(text_pages(ROOT / "SOURCE_CODE_INDEX.md", "SOURCE-CODE PACKAGE"))
pages.extend(text_pages(ROOT / "MANUAL_GUI_TEST_RECORD.md", "MANUAL GUI TEST RECORD"))
pages.extend(source_pages())
pages[0].save(OUT, "PDF", resolution=150.0, save_all=True, append_images=pages[1:])
print(OUT)

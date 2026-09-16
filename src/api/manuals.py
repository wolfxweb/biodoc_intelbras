"""Catálogo dos manuais Markdown do middleware."""

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path

import markdown

_PROJECT_ROOT = Path(__file__).resolve().parents[2]
_SLUG_RE = re.compile(r"^[a-z0-9-]+$")


@dataclass(frozen=True)
class Manual:
    slug: str
    title: str
    path: Path


def _heading(path: Path) -> str:
    try:
        for line in path.read_text(encoding="utf-8").splitlines():
            text = line.strip()
            if text.startswith("# "):
                return text[2:].strip()
    except OSError:
        return path.stem
    return path.stem.replace("_", " ")


def _slug_for(path: Path) -> str:
    if path.name.upper() == "README.MD":
        return "middleware"
    return path.stem.lower().replace("_", "-")


def list_manuals(*, root: Path | None = None) -> list[Manual]:
    base = root or _PROJECT_ROOT
    files: list[Path] = []
    readme = base / "README.md"
    if readme.is_file():
        files.append(readme)
    docs = base / "docs"
    if docs.is_dir():
        files.extend(sorted(path for path in docs.glob("*.md") if path.is_file()))
    manuals: list[Manual] = []
    seen: set[str] = set()
    for path in files:
        slug = _slug_for(path)
        if slug in seen:
            continue
        seen.add(slug)
        manuals.append(Manual(slug=slug, title=_heading(path), path=path))
    return manuals


def get_manual(slug: str, *, root: Path | None = None) -> Manual | None:
    if not _SLUG_RE.match(slug or ""):
        return None
    for manual in list_manuals(root=root):
        if manual.slug == slug:
            return manual
    return None


def render_manual(manual: Manual) -> str:
    body = manual.path.read_text(encoding="utf-8")
    return markdown.markdown(
        body,
        extensions=["tables", "fenced_code", "nl2br"],
    )

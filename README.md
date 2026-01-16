# Record Editor (C)

Tiny terminal app to add, list, search, and delete records with CSV persistence.

## Build & Run
```bash
cd record-editor
make
./record_editor
```
Data is saved to `records.csv` in the same folder.

## Commands
- `1` Add record (auto-increment id)
- `2` List all
- `3` Search by id
- `4` Search by name
- `5` Delete by id
- `0` Quit

## Publish to GitHub
Create a repo named `record-editor` and push:
```bash
cd record-editor
git init
git add .
git commit -m "feat: add record editor (C)"
git branch -M main
git remote add origin https://github.com/ndv0gel/record-editor.git
git push -u origin main
```

Then I can set your portfolio link to:
https://github.com/ndv0gel/record-editor

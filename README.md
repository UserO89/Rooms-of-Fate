"""# 🧙‍♂️ Rooms of Fate — Console RPG in C

## 📅 Day 2 — Player Initialization, Save and Load

In this stage, we added the following core features:

- 📥 Input player name (via argument or prompt)
- 👤 Player initialization (`name`, `health`, `gold`, equipment)
- 💾 Game save (`saveGame`) and load (`loadGame`) support
- 🧪 Basic check for existing saves
- 🧱 Foundation for the game loop is laid

---

## 📂 Key Functions

- `int main()` — handles game startup and player initialization
- `loadGame(Player*, char*)` — loads player data from file
- `saveGame(Player*)` — writes player data to file

---

## 🧪 Example Launch

```bash
./RoomsOfFate Vladislav
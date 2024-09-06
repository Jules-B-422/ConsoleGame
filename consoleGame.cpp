#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>

int pos[] = {1, 1};
int linPos = 43;
int worldSize[] = {20, 20};
std::string world;

// Character skins
std::string s_plr = "x";
std::string s_spc = " ";

std::string loadedlevel;
/*std::string testLevel = "####################\n"
                        "#..................#\n"
                        "#........#.........#\n"
                        "#........#.........#\n"
                        "#........#.........#\n"
                        "####################\n";*/

void GetLevelFile(std::string filePath) {
  std::string levelString = "";
  std::string line;
  std::fstream levelFile(filePath);
  while (std::getline(levelFile, line)) {
    levelString += line + "\n";
  }
  levelFile.close();
  loadedlevel = levelString;
}

void SetLevelDim(std::string inputLevel) {
  int x = 0;
  int y = 0;

  bool gotLength = false;
  for (int i = 0; i < inputLevel.length(); i++) {
    if (inputLevel.substr(i, 2).compare("\n") == true) {
      gotLength = true;
      y++;
    }
    if (gotLength == false) {
      x++;
    }
  }
  worldSize[0] = x;
  worldSize[1] = y;
}

void CreateCanvas() {
  for (int i = 0; i < worldSize[1]; i++) {
    for (int f = 0; f < worldSize[0]; f++) {
      world += ".";
    }
    world += "\n";
  }
}
void LoadLevel(std::string inputLevel) {
  for (int i = 0; i < loadedlevel.length(); i++) {
    world.replace(i, 1, loadedlevel.substr(i, 1));
  }
}

void ApplySkins() {
  for (int i = 0; i < world.length(); i++) {
    // Empty Space replacement
    if (world.substr(i, 1).compare(".") == false) {
      world.replace(i, 1, s_spc);
    }
    // Player skin replacement
    if (world.substr(i, 1).compare("X") == false) {
      world.replace(i, 1, s_plr);
    }
  }
}

bool CheckCollision(int index) {
  if (world.substr(index, 1).compare(".") == false ||
      world.substr(index, 1).compare(" ") == false) {
    return false;
  } else {
    return true;
  }
}

void RenderLevel(std::string level) {
  // Clear screen and reset world
  std::cout << "\033[2J\033[1;1H";
  world = "";
  CreateCanvas();
  LoadLevel(level);
  // Render player
  world.replace(linPos, 1, "X");
  // Apply skins
  ApplySkins();
  std::cout << world;
  std::cout << "Canvas: (" << worldSize[0] << ", " << worldSize[1] << ")";
  std::cout << "\n";
}

int main() {
  // Render for the first time
  std::string inputLevel;
  std::cout << "Choose a level:\ntestLevel\ntownLevel\nasciiText\n";
  std::cin >> inputLevel;
  GetLevelFile("levels/" + inputLevel + ".txt");
  std::string chosenLevel = loadedlevel;
  SetLevelDim(chosenLevel);
  RenderLevel(chosenLevel);

  while (true) {
    // Take commands
    std::string pcmd;
    std::cin >> pcmd;
    // Movement
    if (pcmd == "a") {
      if (CheckCollision(linPos - 1) == false) {
        linPos--;
      }
    }
    if (pcmd == "d") {
      if (CheckCollision(linPos + 1) == false) {
        linPos++;
      }
    }
    if (pcmd == "w") {
      if (CheckCollision(linPos - (worldSize[0] + 1)) == false) {
        linPos -= (worldSize[0] + 1);
      }
    }
    if (pcmd == "s") {
      if (CheckCollision(linPos + (worldSize[0] + 1)) == false) {
        linPos += (worldSize[0] + 1);
      }
    }
    // Close game
    if (pcmd == "x") {
      return (0);
    }

    // Clear screen and regenerate canvas
    RenderLevel(chosenLevel);
    // usleep(10000);
  }
}
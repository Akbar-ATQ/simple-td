

class Spawner
{
public:
//     Spawner(Vector2 pos, std::vector<std::shared_ptr<Enemy>>& enemies, Grid& grid) : data{pos, TILE_SIZE, PURPLE}, enemies{enemies}, grid{grid} {};

//     EntityData data;

//     void Draw() { Tile::DrawRec(data.GetRec(), data.color); };

//     void GenerateEnemy(int waveEnemies, float interval, bool& waveInProgress, AStar::CoordinateList& path)
//     {
//         timer += GetFrameTime();
//         if (timer >= interval)
//         {
//             Vector2 enemyStartingPosition {data.position.x + (GetRandomValue(0, 15) / 100.0f), data.position.y + (GetRandomValue(5, 40) / 100.0f)};

//             std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(enemyStartingPosition, path);
//             enemies.push_back(enemy);
//             grid.map[static_cast<int>(enemy->data.position.x)][static_cast<int>(enemy->data.position.y)].enemies.push_back(enemy);
//             // std::cout << " enemies " << grid.map[static_cast<int>(enemy->data.position.x)][static_cast<int>(enemy->data.position.y)].IsEmpty();

//             generatedEnemy += 1;
//             timer = 0.0f;
//             if (generatedEnemy >= waveEnemies)
//             {
//                 waveInProgress = false;
//             }
//         }
//     };

// private:
//     int generatedEnemy {0};
//     float timer {0};

//     std::vector<std::shared_ptr<Enemy>>& enemies;
//     Grid& grid;
};
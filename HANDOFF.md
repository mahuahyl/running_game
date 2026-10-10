# HANDOFF — running_game

> 交接文档。描述仓库在 **refactor/game-class 分支、工作区有未提交改动**时的状态。
> 行号对应当时实际文件，改动后可能偏移，请以符号名为准。

---

## 1. 项目是什么

SFML 3 的 2D 俯视角跑酷/射击小游戏。玩家（Mask Dude）用鼠标左键朝光标方向发射子弹，
敌人从屏幕随机位置生成并追踪玩家；子弹击杀敌人得分，玩家碰到敌人游戏结束，按 R 重开。

- **语言/标准**：C++17
- **图形库**：SFML 3.0.2（静态链接，`-DSFML_STATIC`）
- **工具链**：MSYS2 MinGW64，`C:\msys64\mingw64\bin\g++.exe`
- **资源**：Kenney "Pixel Adventure" 素材包，175 个文件在 `assets/`
- **窗口**：800×600，帧率上限 240

---

## 2. 怎么构建和运行

### VS Code（项目所有者平时用这个）
`Ctrl + Shift + B` → 跑 `.vscode/tasks.json` 里的 `Build SFML (g++)` 任务，产出 `runrun.exe`。

> ⚠️ 这个任务**没有开任何警告开关**。建议给它加上 `-Wall -Wextra -Wshadow`
> —— 本文件第 5 节里的好几个问题，编译器本来就能直接报出来。

### 命令行
```bash
g++ -std=c++17 -DSFML_STATIC -I C:/msys64/mingw64/include -L C:/msys64/mingw64/lib \
    src/main.cpp -o runrun.exe \
    -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lbz2 -lpng16 \
    -lharfbuzz -lbrotlidec -lbrotlicommon -lz -lgraphite2 -lglib-2.0 -lintl \
    -liconv -lpcre2-8 -lopengl32 -lgdi32 -lwinmm \
    -static-libgcc -static-libstdc++ -mwindows
```

### 只做语法/类型检查（快，不带链接）
```bash
g++ -std=c++17 -DSFML_STATIC -I C:/msys64/mingw64/include \
    -Wall -Wextra -Wshadow -fsyntax-only src/main.cpp
```

### 运行
**必须从项目根目录运行** —— 纹理路径是相对路径（`assets/...`），换目录会因为
`Textures::loadtexture` 失败而弹 MessageBox 并 `std::exit(1)`。

---

## 3. 目录结构

```
running_game/
├── src/                    ← 全部代码（header-only，只有 main.cpp 是 .cpp）
│   ├── main.cpp            6 行：构造 Game，调 run()
│   ├── game.h              Game 类：主循环 / 状态机 / 生成 / 碰撞 / 渲染
│   ├── texture.h           Textures 类：独占全部 sf::Texture
│   ├── character.h         Character 基类（抽象）
│   ├── player.h            Player  : Character
│   ├── follower.h          Follower: Character（**死代码，见 5.9**）
│   ├── enemy.h             Enemy   : Character（中间层）
│   ├── fast_enemy.h        FastE   : Enemy（速度 250 / HP 10）
│   ├── normal_enemy.h      NormalE : Enemy（速度 150 / HP 50）
│   ├── bullet.h            Bullet（**有子类，见 5.2 / 5.3 / 5.4**）
│   ├── boom_bullet.h       BoomB   : Bullet（**未被引用，见 5.3**）
│   ├── normal_bullet.h     NormalB : Bullet（**未被引用，见 5.3**）
│   ├── scorer.h            Scorer：分数显示
│   └── texter.h            Texter：通用文字渲染
├── assets/                 素材（已提交）
├── releases/               历史发布包（已 gitignore）
├── .vscode/tasks.json      构建任务
└── HANDOFF.md              本文件
```

**注意：构建只编译 `src/main.cpp`**，其余全部靠 `#include` 拉起。新增 `.cpp` 必须
手动加进 `tasks.json` 的 `args`。

---

## 4. 架构与关键设计

### 主循环（`Game::run()`，game.h）
```
while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    if (dt > 0.1f) dt = 0.1f;     // 防止拖窗口造成的 dt 尖峰
    update(dt);                    // 生成 + 移动 + 碰撞 + 清理
    processEvents();               // 事件驱动（发射子弹、重开）
    render();                      // clear → draw → display（自成闭环）
}
```

### 状态机
`enum class game_state { playing, over }`，**初始值是 `over`**（见 5.8），
`over` 时显示 "press r to play"，按 R 走 `reset()`。

### 资源所有权
`Textures` 是**唯一**持有 `sf::Texture` 对象的地方。其它所有类只存
`const sf::Texture*`。构造函数里 `p1(textures.p_idle_texture, ...)` 传引用、内部取地址
—— 所以 **`Textures` 必须在 `p1` 之前声明**（当前 game.h 中顺序正确）。

### 已确立的约定（改动时请遵守）

1. **纹理只存指针，绝不按值传递或拷贝。**
2. **成员声明顺序 = 初始化顺序**。`window` 最前，`textures` 早于 `p1`/`n1`/`scorer`/`texter`。
3. **需要多态的容器装 `std::vector<std::unique_ptr<T>>`**（`enemies` 已是）。
4. **解引用规则**：
   - `for (auto &it : vec)` → `it` 是元素引用 → `it->方法()`
   - `for (auto it = vec.begin(); ...)` → `it` 是迭代器 → `(*it)->方法()`
   - 这条踩过坑，见 6.7。
5. 只读方法标 `const`；`this->` 只在发生变量遮蔽时用。
6. 注释和命名混用中英，保持现状即可。

---

## 5. 已知问题（按优先级）

### 5.1 🔴 `Bullet` 的 `damage` 参数被静默丢弃

`src/bullet.h:21-22`

```cpp
Bullet(const sf::Texture &Tex, sf::Vector2f direction, sf::Vector2f position,
       float speed, float damage)
     : b_Tex(&Tex), b_spr(Tex), b_direction(direction), b_position(position), b_speed(speed)
//                                                                        ↑ 没有 damage(damage)
```

成员声明是 `float damage = 10.f;`（bullet.h:10），默认值兜底生效。

**后果**：`BoomB` 请求 100 伤害，实际拿到 10。`NormalB` 因为恰好也传 10 而看不出异常。

**这是本项目第二次犯「初始化列表漏写成员」**（第一次是 `Character::m_Hp`，已修）。
修法：初始化列表补 `damage(damage)`。

> 建议：用 `-Wall -Wextra`（`-Wunused-parameter` 会直接报出来）。

### 5.2 🔴 `bullets` 仍是按值容器，一旦启用子弹子类就会切片

`src/game.h:45` — `std::vector<Bullet> bullets;`

`enemies` 已从 `vector<Enemy>` 改成 `vector<unique_ptr<Enemy>>`，**但 `bullets` 没有**。
当前 game.h 里只构造朴素的 `Bullet`，所以还没暴露；一旦改成 `push_back(BoomB(...))`，
子类部分会被静默切掉。

修法参照 `enemies`：改 `std::vector<std::unique_ptr<Bullet>>`，
构造点用 `std::make_unique<BoomB>(...)`，遍历处 `.` → `->`。

### 5.3 🔴 `NormalB` / `BoomB` 是死代码

`src/normal_bullet.h`、`src/boom_bullet.h` **没有被任何文件 include**
（`game.h` 只 include 了 `bullet.h`）。game.h:217 直接构造裸 `Bullet`。

**这两个类目前从未被实例化**，因此 5.1 的 bug 也还没显形。

### 5.4 🔴 `Bullet` 缺虚析构，且行为方法非虚

`src/bullet.h`

- 没有 `virtual ~Bullet()`。一旦用 `unique_ptr<Bullet>` 持有子类，删除时是 UB。
- `update()` / `check_distance()` 都**不是 virtual**，即使放进 `vector<Bullet*>`
  也不会动态分派。

多态化子弹之前必须先补上。参照 `Character`：`virtual ~Character() = default;`（character.h:104）。

### 5.5 🟡 `Game::reset()` 不重置刷怪计时器

`src/game.h:64-71`。`NspawnTimer` / `FspawnTimer` 没归零。

`FspawnInterval` 只有 2 秒 —— 若玩家在 `FspawnTimer = 1.9` 时死亡，
重开后 0.1 秒就会出现一只 FastE 贴脸。

### 5.6 🟡 `BoomB::bound` 从未被使用

`src/boom_bullet.h:6` — `float bound = 100.f;` 没有任何读取点。
（推测是给「爆炸范围」预留的，尚未实现。）

### 5.7 🟡 `Bullet::update()` 每帧重复归一化，且零长度会产出 NaN

`src/bullet.h:32-34`

```cpp
float len = sqrt(pow(b_direction.x, 2) + pow(b_direction.y, 2));
b_direction.x = b_direction.x / len;   // len == 0 时 → NaN
```

- 归一化只需在构造时做一次，现在每帧都做（浪费）。
- **如果玩家用鼠标点在自己正中心**，`b_dire = (0,0)` → `0/0` → NaN → 精灵位置变 NaN。
  随后 `check_distance()`（`NaN > 40000` 为 false）和 `check_is_alive()` 都为假
  → **这颗子弹永远删不掉**，成为隐形泄漏。

修法：归一化挪进构造函数 + 加 `if (len > 0.f)` 保护。
（对比：`Enemy::getDirection()` 有 `距离 <= 10.f` 的保护，所以没这个问题。）

### 5.8 🟡 `state` 初始值为 `over` —— 需确认是否有意

`src/game.h:35` — `game_state state = game_state::over;`

配合 game.h:91 的文案 `"press r to play"`，看起来是**有意设计成标题画面**。
若属有意，建议把「开场」和「游戏结束」的文案区分开（后者用 `"press r to replay"`）。

### 5.9 🟡 `Follower n1` 是僵尸成员

`src/game.h:42`。构造了（game.h:247），但 `n1.update()` / `n1.draw()`
在 game.h:88 和 196 都是注释状态。要么启用，要么删除。

### 5.10 🟡 刷怪逻辑重复两遍

`src/game.h:104-131`。「随机取一个离玩家足够远的出生点」这段 do-while
连同变量名一字不差地抄了两遍，只有距离阈值（50 / 100）和敌人类型不同。

建议抽成 `sf::Vector2f random_spawn_pos(float min_distance)`。
抽出来之后也更容易判断「为什么快的敌人反而要求出生点更远（100 > 50）」是否合理。

### 5.11 🟢 命名与整洁

| 项 | 位置 |
|---|---|
| `cal_distant` 返回的是**平方**距离，名字误导（调用点用 `10*10`、`20*20` 配合） | game.h:56 |
| 两个完全相同的 `if (state == game_state::playing)` 相邻，应合并 | game.h:100, 135 |
| `p_position` 在循环内重复计算，可提到循环外 | game.h:163 |
| `Bullet::getDamage()` 未标 `const` | bullet.h:47 |
| `Character::reset()` 不重置 `m_frameTimer` | character.h:86-90 |
| `m_team` / `m_state` / `m_face_direc` / `getFacingDir()` 只写不读 | character.h |
| `Texter::draw()` 末尾的 `texts.clear()` 与 `divide()` 开头的重复 | texter.h:18, 57 |
| 未使用的 include：`<iostream>` `<windows.h>` `<algorithm>` | game.h:3-6 |
| 未使用的 include：`<cmath>` | texter.h:3 |
| 未使用的 include：`<windows.h>` | scorer.h:4 |
| `-Wreorder` 警告 ×3（初始化列表顺序与声明顺序不符，当前无害） | character.h:41, bullet.h:22, game.h:250-251 |
| 大量魔法数字：`50.f` `100.f` `10*10` `20*20` `200` `300.f` `12` `15.f` | 全文 |
| `Scorer::divide()` 中 `int score = this->score;` 靠遮蔽工作，建议改名 | scorer.h:15 |

---

## 6. 历史坑位（已修复 —— 不要改回去）

这些是本项目真实踩过并修复的 bug，改动相关代码时请留意别退回旧写法：

1. **`sf::RenderWindow` 默认构造不创建窗口** —— 必须在初始化列表里传 `sf::VideoMode`。
2. **初始化列表漏写成员会被默认值静默兜底** —— `m_Hp(Hp)` 曾漏写，导致所有敌人 HP 都是 100。
   **同类问题目前仍在 `Bullet::damage` 上（见 5.1）。**
3. **纹理必须指向 `Textures` 的成员** —— 曾出现「两套纹理」，绑定了未加载的那套 → 全黑屏。
4. **换纹理时必须同步重置 `textureRect`** —— `Run`(12帧) → `Idle`(11帧) 切换时，
   若 `m_frame` 停在 11 会越界采样，渲染成纯色块。已在 character.h:72-73 修。
5. **`dt` 必须 clamp** —— 拖动/缩放窗口会阻塞消息循环，`dt` 尖峰导致全场瞬移。
6. **多态容器必须装指针** —— `vector<Enemy>` 存储子类会切片。
7. **迭代器与范围 for 的解引用层数不同** —— `vector<unique_ptr<T>>` 中，
   `for (auto& it : v) it->m()` 正确，但 `for (auto it = v.begin(); ...) it->m()`
   会编译失败（`it->` 拿到的是 `unique_ptr` 而非 `T`），需写 `(*it)->m()`。
8. **`sf::Mouse::getPosition()` 必须传窗口** —— 否则返回屏幕坐标而非窗口坐标。
9. **`sf::IntRect(position, size)` 的第二个参数是尺寸**，不是右下角坐标。

---

## 7. Git 状态

- **当前分支**：`refactor/game-class`（领先 `main` 3 个提交，未合并、未推送）

```
3ab0fcc 刺客敌人纹理更换
cc951b9 增加一种刺客型敌人
7da7db5 抽象出game和textures类，简化主循环
2480303 忽略 releases/ 发布产物
4732d7a 停止跟踪 releases/ 发布产物
9486183 增加按R重新开始
a25de06 完善角色死亡，拥有完整游戏流程
cc3ec14 增加分数计算
c8df2d7 优化敌人出现规则（随机出现）
bd8d9db 创建了第一个敌人，具有初步可玩性，子弹可以杀死敌人
ed306bc 玩家新增发射子弹能力
7166100 修复贴图未上传的闪退bug
```

**未提交的工作区改动**：
```
 M src/bullet.h
 M src/character.h
 M src/enemy.h
 M src/game.h
 M src/player.h
?? src/boom_bullet.h
?? src/normal_bullet.h
```

`.gitignore` 覆盖：`build/`、`*.exe`、`*.dll`、`releases/`、`.vscode/`。
历史发布包在 `releases/runrunv*/`（每个含 assets + exe + dll + zip），
已从版本控制移除但保留在磁盘。

---

## 8. 关于项目所有者（协作方式）

- **C++ / SFML 初学者**，学习阶段，使用中文。
- 之前的协作约定：**先讲清原理和「为什么」，再动代码**；不主动改他的源文件，
  只给思路 + 标注「示例，仅供参考」的代码块，由他自己敲。
- 如果他明确说「帮我改」「直接改」，才切换为直接编辑。
- 他重视：**先肯定做得好的地方，再提问题**；建议分级（🔴 会出事 / 🟡 建议考虑 / 🟢 顺手一提）。
- 他反复踩的坑是「构造函数初始化列表漏写」，**每次 review 都值得重点扫一眼**。

---

## 9. 建议的下手顺序

1. `bullet.h` 补 `damage(damage)`（5.1）—— 一行，风险最高
2. 打开 `-Wall -Wextra -Wshadow`，把编译器能抓的都抓出来
3. 决定子弹是否要做成多态：
   - 是 → 补虚析构、`bullets` 换 `unique_ptr`、接入 `NormalB`/`BoomB`（5.2/5.3/5.4）
   - 否 → 删掉那两个文件
4. `reset()` 补两个计时器归零（5.5）
5. `Bullet` 归一化挪进构造函数 + 零长度保护（5.7）
6. 抽 `random_spawn_pos()` 消除重复（5.10）
7. 收尾整洁项（5.11）

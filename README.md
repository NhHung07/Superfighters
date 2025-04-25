# 2-Player Fighting Game - A SDL2/C++ Game

**Tác giả**: Trần Nhật Hưng  
**MSSV**: 24021507

---

- [0. Giới thiệu](#0-giới-thiệu)
- [1. Hình ảnh xem trước](#1-hình-ảnh-xem-trước)
- [2. Cài đặt và chạy game](#2-cài-đặt-và-chạy-game)
- [3. Hướng dẫn chơi](#3-hướng-dẫn-chơi)
- [4. Nhân vật](#4-nhân-vật)
- [5. Điều khiển](#5-điều-khiển)
- [6. Kết luận](#6-kết-luận)

---

## 0. Giới thiệu
Đây là game đối kháng 2 người chơi do mình phát triển bằng **C++** và **SDL2**.  
Game hỗ trợ nhiều tính năng như:  
- Tấn công bằng kiếm  
- Bắn súng  
- Chiêu Ultimate  
- Nhặt vũ khí rơi trên bản đồ  

Dự án này là một phần trong quá trình mình học SDL2 và luyện kỹ năng lập trình game.  
Mong nhận được sự ủng hộ từ thầy/cô và các bạn!

---

## 1. Hình ảnh xem trước

<p align="center">
  <img src="assets/player1_idle_right.png" width="80" />
  <img src="assets/player2_idle_left.png" width="80" />
</p>

> Hình ảnh hai nhân vật chính trong game

---

## 2. Cài đặt và chạy game

- Game được build bằng **Visual Studio 2022**
- Cần cài đặt các thư viện sau:
  - `SDL2`
  - `SDL2_image`
  - `SDL2_mixer`
- Mở file `.sln`, build và chạy `main.cpp`

---

## 3. Hướng dẫn chơi

1. Hai người chơi chiến đấu đến khi một người hết máu.
2. Nhặt vũ khí rơi trên bản đồ để tăng sức mạnh.
3. Vũ khí gồm:
   - **Kiếm**: Tấn công cận chiến
   - **Súng**: Tấn công tầm xa
4. Dùng **Ultimate** khi đủ năng lượng (đánh trúng đối thủ 5 lần).

---

## 4. Nhân vật

| Nhân vật   | Mô tả                   |
|------------|--------------------------|
| Player 1   | Lão già dùng rìu phép    |
| Player 2   | Hiệp sĩ dùng khiên & kiếm |

Hình ảnh chi tiết: xem phần [Hình ảnh xem trước](#1-hình-ảnh-xem-trước)

---

## 5. Điều khiển

### 🎮 Player 1:
- Di chuyển: `W`, `A`, `S`, `D`
- Tấn công bằng kiếm: `J`
- Bắn súng: `K`
- Dùng chiêu Ultimate: `U`

### 🎮 Player 2:
- Di chuyển: `↑`, `←`, `↓`, `→`
- Tấn công bằng kiếm: `1`
- Bắn súng: `2`
- Dùng chiêu Ultimate: `3`

---

## 6. Kết luận

Đây là dự án tâm huyết của mình để thực hành SDL2 và phát triển kỹ năng lập trình game.  
Hi vọng trò chơi sẽ mang lại trải nghiệm thú vị cho người chơi!  

Nếu được điểm tối đa thì xin cảm ơn thầy/cô rất nhiều ạ ❤️

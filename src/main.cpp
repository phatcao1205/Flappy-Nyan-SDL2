#include <iostream>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>

#include "Game.h"
#include "Bird.h"
#include "Pipe.h"
#include "Constants.h"

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(&window, &renderer)) {
        return 1;
    }

    srand(time(0)); // Seed random number generator

    Bird bird = createBird();
    Pipe pipes[4]; // Tạo một mảng các ống
    int numPipes = 4;

    // Khởi tạo vị trí của các ống
    for (int i = 0; i < numPipes; ++i) {
        pipes[i] = createPipe(SCREEN_WIDTH + i * 200); // Các ống cách nhau 200 pixel
    }

    const float gravity = 0.5f;
    const float flap_strength = -10.0f;
    float pipe_speed = -2.0f;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    bird.y_velocity = flap_strength; // Chim bay lên
                }
            }
        }

        // Logic game

        // Cập nhật vị trí của chim
        bird.y_velocity += gravity;
        bird.rect.y += bird.y_velocity;

        // Giới hạn chim trong màn hình
        if (bird.rect.y < 0) {
            bird.rect.y = 0;
            bird.y_velocity = 0;
        }
        if (bird.rect.y > SCREEN_HEIGHT - bird.rect.h) {
            bird.rect.y = SCREEN_HEIGHT - bird.rect.h;
            bird.y_velocity = 0;
            //quit = true; // Kết thúc game khi chạm đáy
        }

        // Di chuyển các ống và tạo ống mới nếu cần
        for (int i = 0; i < numPipes; ++i) {
            pipes[i].top.x += pipe_speed;
            pipes[i].bottom.x += pipe_speed;

            // Nếu ống đi ra khỏi màn hình, tạo lại nó ở cuối
            if (pipes[i].top.x < -pipes[i].top.w) {
                pipes[i] = createPipe(SCREEN_WIDTH + (numPipes -1 ) * 200); // Đặt ống mới sau ống cuối cùng
            }
        }

        // Kiểm tra va chạm
        for (int i = 0; i < numPipes; ++i) {
            if (checkCollision(bird, pipes[i])) {
                std::cout << "Collision detected!" << std::endl;
                quit = true; // Kết thúc game khi va chạm
                break;
            }
        }


        // Vẽ
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, COLOR_BLACK.a);
        SDL_RenderClear(renderer);

        drawBird(renderer, bird);
        for (int i = 0; i < numPipes; ++i) {
            drawPipe(renderer, pipes[i]);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Khoảng 60 FPS
    }

    // Giải phóng tài nguyên và đóng SDL
    close(window, renderer);

    return 0;
}
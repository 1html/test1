#include <stdio.h>
#include <conio.h>  // 키보드 입력 처리에 필요
#include <stdlib.h> // system("cls")를 사용하기 위해 필요
#include <time.h>   // 난수 생성을 위한 시간 관련 함수
#include <windows.h> // Sleep() 함수를 사용하기 위해 필요
#include <stdbool.h> // bool 타입 사용을 위해 필요
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") 

#define WIDTH 20
#define HEIGHT 10
#define OBSTACLE_COUNT 6  // 장애물 개수
#define SLEEP_TIME 10      // 10ms (게임 속도 조절용)

// 색상 정의
#define COLOR_BLACK 0
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_WHITE 15

typedef struct {
    int x, y;     // 장애물의 좌표
    int dx, dy;   // 장애물의 이동 방향 (dx: 가로, dy: 세로)
    int speed;    // 장애물의 속도 (몇 프레임마다 이동할지)
    int frameCount; // 이동 속도에 맞춰 프레임을 카운트
} Obstacle;

int playerPosX = WIDTH / 2;    // 플레이어의 가로 위치 (중앙 위치)
int playerPosY = HEIGHT / 2;    // 플레이어의 세로 위치 (중앙 위치)
int score = 0;                 // 점수 초기화
bool gameOver = false;         // 게임 오버 상태 플래그
Obstacle obstacles[OBSTACLE_COUNT]; // 6개의 장애물
bool showMessage = false;       // 메시지 표시 여부
int messageDisplayTime = 0;     // 메시지 표시 시간 카운트

// 장애물 초기화 (완전히 랜덤 위치와 속도로 생성)
void initObstacles() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        int direction = rand() % 4;  // 0: 왼쪽, 1: 오른쪽, 2: 위쪽, 3: 아래쪽
        if (direction == 0) {  // 왼쪽에서 시작
            obstacles[i].x = 0;             // 왼쪽 끝에서 시작
            obstacles[i].y = rand() % HEIGHT;  // 임의의 높이에서 생성
            obstacles[i].dx = 1;  // 오른쪽으로 이동
            obstacles[i].dy = 0;  // 세로 이동 없음
        }
        else if (direction == 1) {  // 오른쪽에서 시작
            obstacles[i].x = WIDTH - 1;  // 오른쪽 끝에서 시작
            obstacles[i].y = rand() % HEIGHT; // 임의의 높이에서 생성
            obstacles[i].dx = -1;  // 왼쪽으로 이동
            obstacles[i].dy = 0;   // 세로 이동 없음
        }
        else if (direction == 2) {  // 위쪽에서 시작
            obstacles[i].x = rand() % WIDTH;  // 임의의 가로 위치에서 생성
            obstacles[i].y = 0;  // 위쪽 끝에서 시작
            obstacles[i].dx = 0;  // 가로 이동 없음
            obstacles[i].dy = 1;  // 아래로 이동
        }
        else if (direction == 3) {  // 아래쪽에서 시작
            obstacles[i].x = rand() % WIDTH;  // 임의의 가로 위치에서 생성
            obstacles[i].y = HEIGHT - 1;  // 아래쪽 끝에서 시작
            obstacles[i].dx = 0;  // 가로 이동 없음
            obstacles[i].dy = -1;  // 위로 이동
        }
        obstacles[i].speed = (rand() % 5) + 3; // 속도를 3~7 프레임마다 이동하도록 설정
        obstacles[i].frameCount = 0; // 프레임 카운터 초기화
    }
}

void draw() {
    system("cls");  // 콘솔 화면을 지웁니다.

    // 점수 출력
    printf("Score: %d\n", score);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == playerPosY && j == playerPosX) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_YELLOW); // 노란색으로 설정
                printf("P"); // 플레이어 위치
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE); // 색상 재설정
            }
            else {
                bool obstacleExists = false;
                for (int k = 0; k < OBSTACLE_COUNT; k++) {
                    if (i == obstacles[k].y && j == obstacles[k].x) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED); // 빨간색으로 설정
                        printf("O"); // 장애물 위치
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE); // 색상 재설정
                        obstacleExists = true;
                        break;
                    }
                }
                if (!obstacleExists) {
                    printf("."); // 빈 공간
                }
            }
        }
        printf("\n");
    }

    // 점수가 50점 이상일 때 메시지 표시
    if (score >= 50 && messageDisplayTime == 0) {
        showMessage = true; // 메시지 표시 플래그 설정
        messageDisplayTime = 50; // 5초 동안 표시하기 위해 50 프레임 설정
    }

    // 메시지 표시
    if (showMessage) {
        printf("Oh my god...");// 오른쪽에 메시지를 출력
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED); // 빨간색으로 설정
        printf("You will never break this game\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
        messageDisplayTime--;
        if (messageDisplayTime <= 0) {
            showMessage = false; // 메시지 표시 플래그 해제
        }
    }

    if (gameOver) {
        printf("Game Over! Your final score: %d\n", score);
        printf("Press any key to exit.\n");
    }
}

void update() {
    // 장애물 이동
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].frameCount++;

        // 장애물의 이동 속도에 맞춰 프레임마다 이동 여부 결정
        if (obstacles[i].frameCount >= obstacles[i].speed) {
            obstacles[i].x += obstacles[i].dx;
            obstacles[i].y += obstacles[i].dy;
            obstacles[i].frameCount = 0; // 프레임 카운터 초기화

            // 장애물이 화면 밖으로 나가면 반대쪽에서 다시 나타나도록 설정
            if (obstacles[i].x < 0 || obstacles[i].x >= WIDTH || obstacles[i].y < 0 || obstacles[i].y >= HEIGHT) {
                // 점수 증가
                score++; // 점수 증가

                // 새로운 방향 설정
                int direction = rand() % 4;  // 새로운 방향 설정 (0: 왼쪽, 1: 오른쪽, 2: 위쪽, 3: 아래쪽)
                if (direction == 0) {  // 왼쪽에서 시작
                    obstacles[i].x = 0;
                    obstacles[i].y = rand() % HEIGHT;
                    obstacles[i].dx = 1;
                    obstacles[i].dy = 0;
                }
                else if (direction == 1) {  // 오른쪽에서 시작
                    obstacles[i].x = WIDTH - 1;
                    obstacles[i].y = rand() % HEIGHT;
                    obstacles[i].dx = -1;
                    obstacles[i].dy = 0;
                }
                else if (direction == 2) {  // 위쪽에서 시작
                    obstacles[i].x = rand() % WIDTH;
                    obstacles[i].y = 0;
                    obstacles[i].dx = 0;
                    obstacles[i].dy = 1;
                }
                else if (direction == 3) {  // 아래쪽에서 시작
                    obstacles[i].x = rand() % WIDTH;
                    obstacles[i].y = HEIGHT - 1;
                    obstacles[i].dx = 0;
                    obstacles[i].dy = -1;
                }
            }
        }
    }

    // 플레이어 입력 처리
    if (_kbhit()) {
        char key = _getch();
        if (gameOver) {
            // 게임 오버 상태에서 아무 키나 누르면 종료
            return;
        }
        if (key == 'w' && playerPosY > 0) {
            playerPosY--; // 위로 이동
        }
        if (key == 's' && playerPosY < HEIGHT - 1) {
            playerPosY++; // 아래로 이동
        }
        if (key == 'a' && playerPosX > 0) {
            playerPosX--; // 왼쪽으로 이동
        }
        if (key == 'd' && playerPosX < WIDTH - 1) {
            playerPosX++; // 오른쪽으로 이동
        }
    }
}

bool checkCollision() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        if (obstacles[i].x == playerPosX && obstacles[i].y == playerPosY) {
            return true; // 충돌 발생
        }
    }
    return false; // 충돌 없음
}

int main() {
    srand(time(0)); // 난수 생성을 위한 시드 값 설정
    initObstacles(); // 장애물 초기화

    // 배경 음악 재생
    PlaySound(NULL, 0, 0);
    Sleep(10);
    PlaySound(TEXT("C:\\game music\\blackhole2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while (1) {
        draw();    // 화면을 그립니다.
        update();  // 게임 상태를 업데이트합니다.

        // 점수가 50점 이상일 때 BGM 변경
        if (score >= 50) {
            PlaySound(TEXT("C:\\game music\\new_bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 새로운 BGM 재생
        }

        // 충돌 감지
        if (checkCollision()) {
            gameOver = true; // 게임 오버 상태로 설정
            draw(); // 게임 오버 화면 그리기
            Sleep(1000); // 2초 대기 후 종료
            break; // 게임 루프 종료
        }

        Sleep(SLEEP_TIME); // 10ms 대기
    }

    // 배경 음악 중지
    PlaySound(NULL, 0, 0); // 음악 중지

    return 0; // 프로그램 종료
}

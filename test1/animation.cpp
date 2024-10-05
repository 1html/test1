#include <stdio.h>
#include <conio.h>  // Ű���� �Է� ó���� �ʿ�
#include <stdlib.h> // system("cls")�� ����ϱ� ���� �ʿ�
#include <time.h>   // ���� ������ ���� �ð� ���� �Լ�
#include <windows.h> // Sleep() �Լ��� ����ϱ� ���� �ʿ�
#include <stdbool.h> // bool Ÿ�� ����� ���� �ʿ�
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") 

#define WIDTH 20
#define HEIGHT 10
#define OBSTACLE_COUNT 6  // ��ֹ� ����
#define SLEEP_TIME 10      // 10ms (���� �ӵ� ������)

// ���� ����
#define COLOR_BLACK 0
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_WHITE 15

typedef struct {
    int x, y;     // ��ֹ��� ��ǥ
    int dx, dy;   // ��ֹ��� �̵� ���� (dx: ����, dy: ����)
    int speed;    // ��ֹ��� �ӵ� (�� �����Ӹ��� �̵�����)
    int frameCount; // �̵� �ӵ��� ���� �������� ī��Ʈ
} Obstacle;

int playerPosX = WIDTH / 2;    // �÷��̾��� ���� ��ġ (�߾� ��ġ)
int playerPosY = HEIGHT / 2;    // �÷��̾��� ���� ��ġ (�߾� ��ġ)
int score = 0;                 // ���� �ʱ�ȭ
bool gameOver = false;         // ���� ���� ���� �÷���
Obstacle obstacles[OBSTACLE_COUNT]; // 6���� ��ֹ�
bool showMessage = false;       // �޽��� ǥ�� ����
int messageDisplayTime = 0;     // �޽��� ǥ�� �ð� ī��Ʈ

// ��ֹ� �ʱ�ȭ (������ ���� ��ġ�� �ӵ��� ����)
void initObstacles() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        int direction = rand() % 4;  // 0: ����, 1: ������, 2: ����, 3: �Ʒ���
        if (direction == 0) {  // ���ʿ��� ����
            obstacles[i].x = 0;             // ���� ������ ����
            obstacles[i].y = rand() % HEIGHT;  // ������ ���̿��� ����
            obstacles[i].dx = 1;  // ���������� �̵�
            obstacles[i].dy = 0;  // ���� �̵� ����
        }
        else if (direction == 1) {  // �����ʿ��� ����
            obstacles[i].x = WIDTH - 1;  // ������ ������ ����
            obstacles[i].y = rand() % HEIGHT; // ������ ���̿��� ����
            obstacles[i].dx = -1;  // �������� �̵�
            obstacles[i].dy = 0;   // ���� �̵� ����
        }
        else if (direction == 2) {  // ���ʿ��� ����
            obstacles[i].x = rand() % WIDTH;  // ������ ���� ��ġ���� ����
            obstacles[i].y = 0;  // ���� ������ ����
            obstacles[i].dx = 0;  // ���� �̵� ����
            obstacles[i].dy = 1;  // �Ʒ��� �̵�
        }
        else if (direction == 3) {  // �Ʒ��ʿ��� ����
            obstacles[i].x = rand() % WIDTH;  // ������ ���� ��ġ���� ����
            obstacles[i].y = HEIGHT - 1;  // �Ʒ��� ������ ����
            obstacles[i].dx = 0;  // ���� �̵� ����
            obstacles[i].dy = -1;  // ���� �̵�
        }
        obstacles[i].speed = (rand() % 5) + 3; // �ӵ��� 3~7 �����Ӹ��� �̵��ϵ��� ����
        obstacles[i].frameCount = 0; // ������ ī���� �ʱ�ȭ
    }
}

void draw() {
    system("cls");  // �ܼ� ȭ���� ����ϴ�.

    // ���� ���
    printf("Score: %d\n", score);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == playerPosY && j == playerPosX) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_YELLOW); // ��������� ����
                printf("P"); // �÷��̾� ��ġ
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE); // ���� �缳��
            }
            else {
                bool obstacleExists = false;
                for (int k = 0; k < OBSTACLE_COUNT; k++) {
                    if (i == obstacles[k].y && j == obstacles[k].x) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED); // ���������� ����
                        printf("O"); // ��ֹ� ��ġ
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE); // ���� �缳��
                        obstacleExists = true;
                        break;
                    }
                }
                if (!obstacleExists) {
                    printf("."); // �� ����
                }
            }
        }
        printf("\n");
    }

    // ������ 50�� �̻��� �� �޽��� ǥ��
    if (score >= 50 && messageDisplayTime == 0) {
        showMessage = true; // �޽��� ǥ�� �÷��� ����
        messageDisplayTime = 50; // 5�� ���� ǥ���ϱ� ���� 50 ������ ����
    }

    // �޽��� ǥ��
    if (showMessage) {
        printf("Oh my god...");// �����ʿ� �޽����� ���
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED); // ���������� ����
        printf("You will never break this game\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
        messageDisplayTime--;
        if (messageDisplayTime <= 0) {
            showMessage = false; // �޽��� ǥ�� �÷��� ����
        }
    }

    if (gameOver) {
        printf("Game Over! Your final score: %d\n", score);
        printf("Press any key to exit.\n");
    }
}

void update() {
    // ��ֹ� �̵�
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacles[i].frameCount++;

        // ��ֹ��� �̵� �ӵ��� ���� �����Ӹ��� �̵� ���� ����
        if (obstacles[i].frameCount >= obstacles[i].speed) {
            obstacles[i].x += obstacles[i].dx;
            obstacles[i].y += obstacles[i].dy;
            obstacles[i].frameCount = 0; // ������ ī���� �ʱ�ȭ

            // ��ֹ��� ȭ�� ������ ������ �ݴ��ʿ��� �ٽ� ��Ÿ������ ����
            if (obstacles[i].x < 0 || obstacles[i].x >= WIDTH || obstacles[i].y < 0 || obstacles[i].y >= HEIGHT) {
                // ���� ����
                score++; // ���� ����

                // ���ο� ���� ����
                int direction = rand() % 4;  // ���ο� ���� ���� (0: ����, 1: ������, 2: ����, 3: �Ʒ���)
                if (direction == 0) {  // ���ʿ��� ����
                    obstacles[i].x = 0;
                    obstacles[i].y = rand() % HEIGHT;
                    obstacles[i].dx = 1;
                    obstacles[i].dy = 0;
                }
                else if (direction == 1) {  // �����ʿ��� ����
                    obstacles[i].x = WIDTH - 1;
                    obstacles[i].y = rand() % HEIGHT;
                    obstacles[i].dx = -1;
                    obstacles[i].dy = 0;
                }
                else if (direction == 2) {  // ���ʿ��� ����
                    obstacles[i].x = rand() % WIDTH;
                    obstacles[i].y = 0;
                    obstacles[i].dx = 0;
                    obstacles[i].dy = 1;
                }
                else if (direction == 3) {  // �Ʒ��ʿ��� ����
                    obstacles[i].x = rand() % WIDTH;
                    obstacles[i].y = HEIGHT - 1;
                    obstacles[i].dx = 0;
                    obstacles[i].dy = -1;
                }
            }
        }
    }

    // �÷��̾� �Է� ó��
    if (_kbhit()) {
        char key = _getch();
        if (gameOver) {
            // ���� ���� ���¿��� �ƹ� Ű�� ������ ����
            return;
        }
        if (key == 'w' && playerPosY > 0) {
            playerPosY--; // ���� �̵�
        }
        if (key == 's' && playerPosY < HEIGHT - 1) {
            playerPosY++; // �Ʒ��� �̵�
        }
        if (key == 'a' && playerPosX > 0) {
            playerPosX--; // �������� �̵�
        }
        if (key == 'd' && playerPosX < WIDTH - 1) {
            playerPosX++; // ���������� �̵�
        }
    }
}

bool checkCollision() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        if (obstacles[i].x == playerPosX && obstacles[i].y == playerPosY) {
            return true; // �浹 �߻�
        }
    }
    return false; // �浹 ����
}

int main() {
    srand(time(0)); // ���� ������ ���� �õ� �� ����
    initObstacles(); // ��ֹ� �ʱ�ȭ

    // ��� ���� ���
    PlaySound(NULL, 0, 0);
    Sleep(10);
    PlaySound(TEXT("C:\\game music\\blackhole2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while (1) {
        draw();    // ȭ���� �׸��ϴ�.
        update();  // ���� ���¸� ������Ʈ�մϴ�.

        // ������ 50�� �̻��� �� BGM ����
        if (score >= 50) {
            PlaySound(TEXT("C:\\game music\\new_bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // ���ο� BGM ���
        }

        // �浹 ����
        if (checkCollision()) {
            gameOver = true; // ���� ���� ���·� ����
            draw(); // ���� ���� ȭ�� �׸���
            Sleep(1000); // 2�� ��� �� ����
            break; // ���� ���� ����
        }

        Sleep(SLEEP_TIME); // 10ms ���
    }

    // ��� ���� ����
    PlaySound(NULL, 0, 0); // ���� ����

    return 0; // ���α׷� ����
}

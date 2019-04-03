#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
// Программа для вызова команды ll в directory
int main(int argc, char *argv[], char *envp[])
{
    struct stat sb;
    char *dir_str;
    if (argc == 2) {
        dir_str = argv[1];
    } else {
        dir_str = "./";
    }


    DIR *dir = opendir(dir_str);
    if (dir == NULL) {
        printf("error opendir\n");
        exit(-1);
    }

    struct dirent *element;
    char name_pass[512];
    char link_buf[512];
    int i, rights;
    struct passwd *pws;
    struct group *grp;

    while ((element = readdir(dir)) != NULL) {
        snprintf(name_pass, 512, "%s%s", dir_str, element->d_name);
        if (lstat(name_pass, &sb) < 0) {
            printf("error lstat\n");
            exit(-1);
        }

        // Тип файла
        switch (sb.st_mode & S_IFMT) {
            case S_IFBLK: printf("%9s", "block_dev"); break;
            case S_IFCHR: printf("%9s", "char_dev"); break;
            case S_IFDIR: printf("%9s", "directory"); break;
            case S_IFIFO: printf("%9s", "fifo/pipe"); break;
            case S_IFLNK:
                printf("%9s", "symlink");
                if (readlink(name_pass, link_buf, 512) < 0) {
                    printf("error lstat\n");
                    exit(-1);
                }
                break;
            case S_IFREG: printf("%9s", "reg_file"); break;
            case S_IFSOCK: printf("%9s", "socket"); break;
            default: printf("%9s", "unknown"); break;
        }
        printf(" --- ");
        // Количество жестких ссылок
        printf("%3ld", sb.st_nlink);

        printf(" --- ");
        // Права доступа
        for(i = 8; i >= 0; --i) {
            rights = (sb.st_mode >> i) & 1;
            if (rights == 1) {
                switch (i % 3) {
                    case 2: printf("r"); break;
                    case 1: printf("w"); break;
                    case 0: printf("x"); break;
                }
            } else {
                printf("-");
            }
        }

        printf(" --- ");
        // Владелец
        pws = getpwuid(sb.st_uid);
        printf("%8s", pws->pw_name);

        printf(" --- ");
        // Владелец
        grp = getgrgid(sb.st_gid);
        printf("%8s", grp->gr_name);

        printf(" --- ");
        // Размер в байтах
        printf("%6ld", sb.st_size);

        printf(" --- ");
        // Дата
        long int s_time;
        struct tm *m_time;
        char str_t[128];
        s_time = sb.st_ctime;
        m_time = localtime(&s_time);
        strftime(str_t, 128, "%c", m_time);
        printf("%s", str_t);

        printf(" --- ");
        // Имя
        printf("%s", element->d_name);
        if (S_ISLNK(sb.st_mode)) {
            printf("--> %s", link_buf);
        }

        printf("\n");

        memset(link_buf, 0, 511);
    }

    if (closedir(dir) == -1){
        printf("error closedir\n");
        exit(-1);
    }

    return 0;
}
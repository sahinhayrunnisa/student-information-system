/*
  240710065 - Hayrunnisa ÞAHÝN
  Atatürk Üniversitesi -> Mühendislik Fakültesi
  Yazýlým Mühendisliði -> Algoritma ve Programlamaya Giriþ
  DÖNEM PROJESÝ –> ÖÐRENCÝ BÝLGÝ SÝSTEMÝ 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DATAFILE "ogrenci_kayitlari.txt"
#define BUFFER_SIZE 256

typedef struct {
    char ad[50];
    int numara;
    char soyad[50];
    char bolum[50];
} Student;

Student *students = NULL;
unsigned long student_count = 0;
unsigned long student_capacity = 0;

void load_from_file(void);
void dosya_kaydet(void);
void ensure_capacity(void);
int find_index_by_num(int num);
int exists_num(int num);
void ogrenci_ekle(void);
void ogrenci_sil(void);
void ogrenci_guncelle(void);
void ogrenci_listele(void);
void ogrenci_ara(void);
void ogrenci_raporla(void);
void numaraya_gore_sirala(void);
void clear_input_buffer(void);
void trim(char *s);

int main(void) {
    int secim;
    load_from_file();

    while (1) {
        printf("\n****** OBS MENU ******\n");
        printf("1. Ogrenci Ekle\n2. Ogrenci Sil\n3. Ogrenci Guncelle\n4. Ogrenci Listele\n5. Ogrenci Ara\n6. Rapor\n7. Kaydet\n0. Cikis\n");
        printf("Seciminiz: ");

        if (scanf("%d", &secim) != 1) {
            printf("Hata: GECERSIZ GIRIS!!!!\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (secim) {
            case 1: ogrenci_ekle(); break;
            case 2: ogrenci_sil(); break;
            case 3: ogrenci_guncelle(); break;
            case 4: ogrenci_listele(); break;
            case 5: ogrenci_ara(); break;
            case 6: ogrenci_raporla(); break;
            case 7: dosya_kaydet(); printf("Veriler dosyaya yazildi.\n"); break;
            case 0:
                dosya_kaydet();
                free(students);
                printf("Program kapatiliyor...\n");
                return 0;
            default:
                printf("GECERSIZ SECIM!\n");
        }
    }
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void trim(char *s) {
    char *p = s;
    unsigned long len;
    while (isspace((unsigned char)*p)) p++;
    memmove(s, p, strlen(p) + 1);
    len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

void ensure_capacity(void) {
    if (student_capacity == 0) {
        student_capacity = 4;
        students = (Student *)malloc(student_capacity * sizeof(Student));
    } else if (student_count >= student_capacity) {
        student_capacity *= 2;
        Student *temp = (Student *)realloc(students, student_capacity * sizeof(Student));
        if (!temp) { exit(EXIT_FAILURE); }
        students = temp;
    }
}

int find_index_by_num(int num) {
    unsigned long i;
    for (i = 0; i < student_count; i++) {
        if (students[i].numara == num) return (int)i;
    }
    return -1;
}

int exists_num(int num) {
    return find_index_by_num(num) != -1;
}

void ogrenci_ekle(void) {
    Student s;
    char buffer[BUFFER_SIZE];

    printf("Numara: ");
    if (scanf("%d", &s.numara) != 1) { clear_input_buffer(); return; }
    clear_input_buffer();

    if (exists_num(s.numara)) {
        printf("Hata: Bu numara zaten VAR!!!\n");
        return;
    }

    printf("Ad: ");
    fgets(buffer, sizeof(buffer), stdin); trim(buffer);
    strncpy(s.ad, buffer, 49);

    printf("Soyad: ");
    fgets(buffer, sizeof(buffer), stdin); trim(buffer);
    strncpy(s.soyad, buffer, 49);

    printf("Bolum: ");
    fgets(buffer, sizeof(buffer), stdin); trim(buffer);
    strncpy(s.bolum, buffer, 49);

    ensure_capacity();
    students[student_count++] = s;
    numaraya_gore_sirala();
    printf("Ogrenci eklendi.\n");
}

void ogrenci_sil(void) {
    int num, idx;
    unsigned long i;
    printf("Silinecek No: ");
    scanf("%d", &num); clear_input_buffer();

    idx = find_index_by_num(num);
    if (idx == -1) { printf("OGRENCI BULUNAMADI!!!!\n"); return; }

    for (i = (unsigned long)idx; i < student_count - 1; i++)
        students[i] = students[i + 1];

    student_count--;
    printf("Ogrenci silindi.\n");
}

void ogrenci_guncelle(void) {
    int num, idx;
    char buffer[BUFFER_SIZE];

    printf("Guncellenecek No: ");
    scanf("%d", &num); clear_input_buffer();

    idx = find_index_by_num(num);
    if (idx == -1) { printf("Bulunamadi!\n"); return; }

    printf("Yeni Ad (Bos gecmek icin Enter): ");
    fgets(buffer, sizeof(buffer), stdin); trim(buffer);
    if (strlen(buffer) > 0) strncpy(students[idx].ad, buffer, 49);

    printf("Yeni Soyad (Bos gecmek icin Enter): ");
    fgets(buffer, sizeof(buffer), stdin); trim(buffer);
    if (strlen(buffer) > 0) strncpy(students[idx].soyad, buffer, 49);

    printf("Yeni Bolum (Bos gecmek icin Enter): ");
    fgets(buffer, sizeof(buffer), stdin); trim(buffer);
    if (strlen(buffer) > 0) strncpy(students[idx].bolum, buffer, 49);
}

void ogrenci_listele(void) {
    unsigned long i;
    if (student_count == 0) { printf("LISTE BOS.\n"); return; }
    for (i = 0; i < student_count; i++) {
        printf("%d - %s %s (%s)\n", 
            students[i].numara, students[i].ad, students[i].soyad, students[i].bolum);
    }
}

void ogrenci_ara(void) {
    int num, idx;
    printf("Aranan No: ");
    scanf("%d", &num); clear_input_buffer();

    idx = find_index_by_num(num);
    if (idx != -1)
        printf("Sonuc: %d %s %s %s\n", students[idx].numara, students[idx].ad, students[idx].soyad, students[idx].bolum);
    else
        printf("BULUNAMADI!\n");
}

void ogrenci_raporla(void) {
    printf("Sistemdeki toplam ogrenci sayisi: %lu\n", student_count);
}

void numaraya_gore_sirala(void) {
    unsigned long i;
    for (i = 1; i < student_count; i++) {
        Student key = students[i];
        int j = (int)i - 1;
        while (j >= 0 && students[j].numara > key.numara) {
            students[j + 1] = students[j];
            j--;
        }
        students[j + 1] = key;
    }
}

void load_from_file(void) {
    FILE *f = fopen(DATAFILE, "r");
    char line[BUFFER_SIZE];
    if (!f) return;
    while (fgets(line, sizeof(line), f)) {
        trim(line);
        Student s;
        if (sscanf(line, "%d,%49[^,],%49[^,],%49[^\n]", &s.numara, s.ad, s.soyad, s.bolum) == 4) {
            ensure_capacity();
            students[student_count++] = s;
        }
    }
    fclose(f);
}

void dosya_kaydet(void) {
    FILE *f = fopen(DATAFILE, "w");
    unsigned long i;
    if (!f) return;
    for (i = 0; i < student_count; i++) {
        fprintf(f, "%d,%s,%s,%s\n", students[i].numara, students[i].ad, students[i].soyad, students[i].bolum);
    }
    fclose(f);
}

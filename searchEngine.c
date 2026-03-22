#include <stdio.h>   // Standart giriş-çıkış (printf, scanf, dosya işlemleri) için gerekli.
#include <string.h>  // Metin işlemleri (strlen, strcpy, strcmp) fonksiyonlarını içerir.
#include <time.h>    // Programın çalışma süresini ölçmek için clock() fonksiyonunu ekler.
#include <ctype.h>   // Karakter dönüşümleri (tolower) ve kontrolleri (isalnum) için kullanılır.
#include <stdlib.h>  // Genel amaçlı fonksiyonlar ve bellek yönetimi için eklenmiştir.

#define MAX_LINE 1024      // Bir seferde okunacak maksimum satır uzunluğu.
#define MAX_FILES 10       // Kullanıcının sisteme girebileceği maksimum dosya sayısı.
#define MAX_FILENAME 100   // Dosya isimlerinin sahip olabileceği maksimum karakter sınırı.

// --- Fonksiyon Prototipleri ---
void toLowerCase(char *str); // Metni küçük harfe çeviren fonksiyon.
int isSeparator(char c);     // Karakterin ayırıcı (nokta, boşluk vb.) olup olmadığını kontrol eder.
void computeLPS(char *pattern, int M, int *lps); // KMP algoritması için ön hazırlık tablosu.
int KMPSearchCount(char *pattern, char *text, int exactMatch); // KMP arama ve sayma motoru.

int main() {
    char filenames[MAX_FILES][MAX_FILENAME]; // Dosya isimlerini depolayan iki boyutlu karakter dizisi.
    char pattern[100];                       // Kullanıcının arayacağı kelimeyi tutan dizi.
    int fileCount = 0;                       // Kaç adet dosya girildiğini takip eden sayaç.
    int searchOption;                        // Arama türü (Normal/Tam Kelime) seçim değişkeni.
    int grandTotal = 0;                      // Tüm dosyalardaki toplam eşleşme sayısı.

    printf("--- METIN ARAMA MOTORU (KMP tabanli) ---\n");

    // 1. ADIM: Dosya isimlerini kullanıcıdan toplama
    while (fileCount < MAX_FILES) { // Maksimum dosya sınırına kadar döngü çalışır.
        printf("%d. Dosya adini girin (Bitirmek icin '0' basin): ", fileCount + 1);
        scanf("%s", filenames[fileCount]); // Dosya ismini oku ve diziye kaydet.
        if (strcmp(filenames[fileCount], "0") == 0) break; // Kullanıcı '0' girerse veri girişini bitir.
        fileCount++; // Dosya sayısını bir artır.
    }

    if (fileCount == 0) { // Eğer hiç dosya girilmeden bitirildiyse programı kapat.
        printf("Hic dosya girilmedi. Program kapatiliyor.\n");
        return 0;
    }

    // 2. ADIM: Arama parametrelerini alma
    printf("\nAranacak kelimeyi girin: ");
    scanf("%s", pattern); // Aranacak kelimeyi kullanıcıdan al.
    
    printf("\nArama Secenegi:\n[1] Normal Arama (Kelimelerin icindekileri de bulur)\n[2] Tam Kelime Aramasi (Sadece bagimsiz kelimeleri bulur)\nSeciminiz: ");
    scanf("%d", &searchOption); // Arama modunu seçtir.
    int exactMatch = (searchOption == 2); // Seçim 2 ise tam eşleşme mantıksal olarak doğru (1) olur.

    // Rapor dosyasını 'w' (yazma) modunda aç. Yoksa oluşturur, varsa içeriğini temizler.
    FILE *reportFile = fopen("search_report.txt", "w");
    if (reportFile == NULL) { // Dosya sistem hatası nedeniyle oluşturulamazsa hata ver.
        printf("Rapor dosyasi olusturulamadi!\n");
        return 1;
    }

    toLowerCase(pattern); // Aranan kelimeyi küçük harfe sabitle (Büyük-küçük harf duyarlılığını kaldırmak için).

    clock_t start = clock(); // İşlem başlangıç zamanını kaydet.

    // Başlıkları konsola ve rapor dosyasına yazdır.
    printf("\n================ DETAYLI RAPOR ================\n");
    fprintf(reportFile, "================ DETAYLI RAPOR ================\n");
    fprintf(reportFile, "Aranan Kelime: %s\n", pattern);
    fprintf(reportFile, "Arama Modu: %s\n\n", exactMatch ? "Tam Kelime" : "Normal");

    // 3. ADIM: Her dosya için tek tek işlem yapma
    for (int f = 0; f < fileCount; f++) {
        FILE *file = fopen(filenames[f], "r"); // Dosyayı sadece okuma ('r') modunda aç.
        if (file == NULL) { // Dosya bulunamazsa veya açılamazsa kullanıcıyı uyar.
            printf("\n[!] HATA: '%s' dosyasi acilamadi!\n", filenames[f]);
            fprintf(reportFile, "\n[!] HATA: '%s' dosyasi acilamadi!\n", filenames[f]);
            continue; // Bu dosyayı atla, bir sonrakine geç.
        }

        int fileFrequency = 0; // Bu dosyadaki toplam bulma sayısı.
        int lineNumber = 0;    // Satır takibi için sayaç.
        char line[MAX_LINE];   // Dosyadan okunan ham satırı tutan tampon (buffer).

        printf("\n>>> DOSYA: %s\n", filenames[f]);
        fprintf(reportFile, "\n>>> DOSYA: %s\n", filenames[f]);

        // fgets: Dosyadan satır satır okur. MAX_LINE sınırı ile bellek taşmasını önle.
        while (fgets(line, MAX_LINE, file)) {
            lineNumber++; // Satır numarasını artır.
            char tempLine[MAX_LINE]; // Orijinal satırı bozmamak için geçici bir kopya oluştur.
            strcpy(tempLine, line); // Satırı kopyala.
            toLowerCase(tempLine); // Kopyayı analiz için küçük harfe çevir.

            // KMP algoritmasını çağırarak bu satırdaki eşleşme sayısını al.
            int count = KMPSearchCount(pattern, tempLine, exactMatch);
            
            if (count > 0) { // Eğer eşleşme bulunduysa detayları yazdır.
                printf("  - Satir %d: %d kez bulundu.\n", lineNumber, count);
                fprintf(reportFile, "  - Satir %d: %d kez bulundu.\n", lineNumber, count);
                fileFrequency += count; // Dosya toplamına ekle.
            }
        }

        printf("--- Bu dosyada toplam: %d kez gecti.\n", fileFrequency);
        fprintf(reportFile, "--- Bu dosyada toplam: %d kez gecti.\n", fileFrequency);
        grandTotal += fileFrequency; // Genel toplama ekle.
        fclose(file); // Açılan dosyayı kapat (Bellek yönetimi için kritik).
    }

    clock_t end = clock(); // İşlem bitiş zamanını kaydet.
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC; // Geçen süreyi saniyeye çevir.

    // 4. ADIM: Genel sonuçları raporlama
    printf("\n================ OZET SONUC ================\n");
    printf("Taramada Gecen Toplam Sure: %.4f saniye\n", time_spent);
    printf("Tum Dosyalardaki Genel Toplam Frekans: %d\n", grandTotal);
    printf("Sonuclar 'search_engine.txt' dosyasina kaydedildi.\n");
    printf("============================================\n");

    fprintf(reportFile, "\n================ OZET SONUC ================\n");
    fprintf(reportFile, "Taramada Gecen Toplam Sure: %.4f saniye\n", time_spent);
    fprintf(reportFile, "Tum Dosyalardaki Genel Toplam Frekans: %d\n", grandTotal);
    fprintf(reportFile, "============================================\n");

    fclose(reportFile); // Rapor dosyasını kapat.
    return 0; // Programı başarıyla sonlandır.
}

// Metni küçük harfe dönüştürür. 'unsigned char' cast işlemi geniş karakter desteği sağlar.
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = (unsigned char)tolower(str[i]);
    }
}

// Karakter harf veya rakam değilse 1 döndürür. Tam kelime aramasında sınırları belirler.
int isSeparator(char c) {
    return !isalnum((unsigned char)c);
}

// KMP'nin en önemli noktası: Pattern içindeki tekrarlayan kısımları bir tabloya dök.
void computeLPS(char *pattern, int M, int *lps) {
    int length = 0; // En uzun önsöz-sonsöz uzunluğu.
    lps[0] = 0;     // İlk karakterin her zaman 0'dır.
    int i = 1;

    while (i < M) {
        if (pattern[i] == pattern[length]) { // Karakterler eşleşiyorsa
            length++;
            lps[i] = length; // Tabloya eşleşme uzunluğunu yaz.
            i++;
        } else { // Eşleşme bozulursa...
            if (length != 0) {
                length = lps[length - 1]; // Önceki başarılı eşleşmeye geri zıpla.
            } else {
                lps[i] = 0; // Hiç eşleşme yoksa 0 yaz ve devam et.
                i++;
            }
        }
    }
}

// KMP Arama Fonksiyonu: Metin içinde pattern'ı arar, 'exactMatch' kontrolünü yapar.
int KMPSearchCount(char *pattern, char *text, int exactMatch) {
    int M = strlen(pattern); // Aranan kelime uzunluğu.
    int N = strlen(text);    // Metin (satır) uzunluğu.
    if (M == 0) return 0;    // Boş kelime aranmaz.

    int lps[M]; // LPS tablosu için yer ayır.
    computeLPS(pattern, M, lps); // Tabloyu doldur.

    int i = 0, j = 0, count = 0; // i:metin imleci, j:pattern imleci, count:sayaç.

    while (i < N) {
        if (pattern[j] == text[i]) { // Karakterler eşleştiği sürece ilerle.
            i++; j++;
        }

        if (j == M) { // j değeri kelime uzunluğuna ulaştıysa: Kelime bulundu!
            int isExact = 1; // Varsayılan: Bulunan şey bir tam kelime.

            if (exactMatch) { // Eğer kullanıcı "Tam Kelime" modunu seçtiyse:
                // Kelimenin hemen solunda harf/rakam varsa tam kelime değildir.
                if ((i - M > 0) && !isSeparator(text[i - M - 1])) isExact = 0;
                // Kelimenin hemen sağında harf/rakam varsa tam kelime değildir.
                if (i < N && !isSeparator(text[i])) isExact = 0;
            }

            if (isExact) count++; // Tüm kontrollerden geçtiyse sayacı artır.
            j = lps[j - 1];       // KMP sayesinde metinde geri gitmeden bir sonraki olası eşleşmeye zıpla.
        } 
        else if (i < N && pattern[j] != text[i]) { // Eşleşme bozulduğunda:
            if (j != 0) j = lps[j - 1]; // Pattern üzerinde LPS tablosuna göre geri git.
            else i++; // Eğer pattern'ın en başındaysak metni bir ilerlet.
        }
    }
    return count; // Bulunan toplam eşleşme sayısını döndür.
}
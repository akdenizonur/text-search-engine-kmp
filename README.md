# Metin Arama Motoru (KMP Algoritması - C)

Bu proje, C dili kullanılarak geliştirilmiş basit bir metin arama motorudur.
Arama işlemi için KMP (Knuth-Morris-Pratt) algoritması kullanılmıştır.

## Projenin Amacı

Bu projeyi yapmamızdaki amaç, metin arama algoritmalarını daha iyi anlamak ve
özellikle KMP algoritmasının nasıl çalıştığını uygulamalı olarak görmekti. Aynı 
zamanda dosya işlemleri ve string işlemleri üzerine pratik yapma fırsatı oldu.

## Özellikler

* Birden fazla dosya üzerinde arama yapabilme
* Büyük/küçük harf duyarsız arama
* İki farklı arama modu:

  * Normal arama (kelimenin geçtiği her yeri bulur)
  * Tam kelime araması (sadece bağımsız kelimeleri bulur)
* Satır bazlı sonuç gösterimi
* Toplam tekrar sayısını hesaplama
* Sonuçları dosyaya yazdırma
* Programın çalışma süresini ölçme

## Nasıl Çalışır?

Program çalıştığında kullanıcıdan:

1. Arama yapılacak dosya isimleri alınır
2. Aranacak kelime girilir
3. Arama türü seçilir
4. Dosyalar satır satır okunur
5. KMP algoritması kullanılarak eşleşmeler bulunur
6. Sonuçlar hem ekrana hem de bir rapor dosyasına yazdırılır

## Kullanılan Algoritma

Bu projede **KMP (Knuth-Morris-Pratt)** algoritması kullanılmıştır.

* LPS (Longest Prefix Suffix) dizisi oluşturulur
* Gereksiz karşılaştırmaların önüne geçilir
* Daha hızlı arama yapılır

## Çıktılar

Program çalıştıktan sonra:

* Konsolda detaylı sonuçlar gösterilir
* `search_report.txt` adlı dosyaya sonuçlar kaydedilir

## Derleme ve Çalıştırma

Aşağıdaki komutlarla programı çalıştırabilirsiniz:

```bash
gcc searchEngine.c -o searchEngine
./searchEngine
```

## Notlar

* Maksimum 10 dosya ile çalışacak şekilde ayarlanmıştır
* Satır uzunluğu en fazla 1024 karakter olabilir
* Sadece metin (.txt) dosyaları ile çalışır

## Geliştiren

    Bu proje, algoritmalar ve C programlama pratiği yapmak amacıyla üniversite
öğrencileri tarafından hazırlanmıştır.


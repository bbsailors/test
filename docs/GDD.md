# GUARDIANS OF THE DEEP - Game Design Document (GDD)

## 🌊 OYUN HİKAYESİ (LORE)
Okyanusların derinliklerinde, ışığın zor ulaştığı Aqualis krallığında denge bozuluyor. Deniz halkı için umut sembolü olan Prenses Aria, okyanusun enerjisini hissedebilen son muhafızdır. Ancak karanlık derinliklerden yükselen **Voidborn** yaratıkları, bu enerjiyi emerek okyanusu karanlığa sürüklemek istiyor.

**Amaç:** Voidborn ordularını durdurmak ve Prenses Aria'yı (ve dolayısıyla denizin kalbini) korumak.

Karanlık derinliklerden gelen bu saldırı dalgalar hâlinde gerçekleşir. Her dalga, Aqualis'in kalbine biraz daha yaklaşan Voidborn ordusunu temsil eder. Oyuncu, Aria'ya ulaşılmadan önce tüm dalgaları durdurmak zorundadır.

## 🛡 OYUNCUNUN ROLÜ
Oyuncu, "Aqualis'in Koruyucusu" rolündedir. Aria'yı doğrudan kontrol etmez; savunma kuleleri inşa ederek düşman dalgalarını durdurur.

- Aria haritanın sonunda, kutsal bölgede durur.
- Düşmanlar haritanın girişinden çıkar, yolu takip ederek Aria'ya ulaşmaya çalışır.
- Oyuncu, yolun kenarlarına kule kurarak bu akını durdurmaya çalışır.

Kısa özet: Oyuncu, deniz krallığı Aqualis’i tehdit eden Voidborn dalgalarına karşı savunma kuleleri kurarak Prenses Aria’yı korur. Her dalga artan zorlukla gelir ve oyuncunun stratejik kararları krallığın kaderini belirler.

## 🏰 KULELER (TOWERS)
1. **Coral Cannon**  
	 - Tür: Tek hedefli hasar kulesi  
	 - Rol: Zayıf ama çok sayıda düşmanı tek tek eritmek  
	 - Özellikler: Orta menzil, orta atış hızı, tek hedefe yüksek hasar  

2. **Tidal Burst Tower**  
	 - Tür: Alan hasarı veren kule (Splash / AoE)  
	 - Rol: Sıkışık gruplar hâlinde ilerleyen Voidborn sürülerini inceltmek  
	 - Özellikler: Daha yavaş atış hızı, yüksek alan hasarı, kısa/orta menzil  

3. **Frost Current Totem**  
	 - Tür: Yavaşlatma (slow) kulesi  
	 - Rol: Düşmanları yavaşlatarak diğer kulelere daha fazla vurma fırsatı vermek  
	 - Özellikler: Düşük hasar veya hiç hasar yok, geniş menzil, güçlü yavaşlatma etkisi

Her kule okyanus enerjisiyle çalışır ve Aria’yı korumak için inşa edilmiştir. Oyuncu, sınırlı para ile bu kulelerden stratejik kombinasyonlar kurmak zorundadır.

## 🐙 DÜŞMANLAR (ENEMIES)
- **Void Walker**  
	- Yavaş ama dayanıklı bir temel düşman.  
	- Yüksek can puanına sahiptir, kulelerden çok darbe yer.  
	- İlk dalgalarda sıkça görülür; oyuncuyu tek hedefli kule kullanmaya zorlar.

- **Abyss Rusher**  
	- Çok hızlı ama canı düşüktür.  
	- Kule menzillerini çabuk geçerek Aria’ya ulaşmaya çalışır.  
	- Oyuncunun iyi yerleştirilmiş yavaşlatma ve alan hasarı kombinasyonu kullanmasını gerektirir.

- **Abyss Lord (Boss)**  
	- Son dalgada gelen devasa tehdit.  
	- Çok yüksek can, yavaş hız.  
	- Özel dalga öncesi uyarı metni ile duyurulur.  
	- Düşerse, Aqualis kurtulmuş sayılır.

## 🔁 OYNANIŞ MEKANİKLERİ

### Dalga Sistemi (Waves)
- Oyun, artan zorlukta birden fazla dalgadan oluşur.  
- Her dalgada:  
	- Daha fazla sayıda düşman çıkar.  
	- Düşmanların hızları ve canları artar.  
- Bazı özel dalgalarda "elite" düşmanlar veya karma kompozisyonlar bulunur.  
- Son dalga, Abyss Lord boss'unun göründüğü final dalgasıdır.

### Ekonomi (Para Sistemi)
- Düşman öldürmek, oyuncuya para kazandırır.  
- Kule kurmak ve yükseltmek (ileride gerekirse) para harcatır.  
- Para yetmiyorsa yeni kule kurulamayıp oyuncu beklemek zorunda kalır.  
- Strateji: Hangi kuleyi ne zaman kuracağına karar vermek, oyunun zorluğunu belirler.

### Can Sistemi
- Aria / Base için belirli bir can (HP) değeri bulunur.  
- Her düşman Aria’ya ulaştığında bu can azalır.  
- Can 0’a düşerse oyun kaybedilir.

### Kazanma & Kaybetme Koşulları
- **Kazanma:**  
	- Tüm dalgalar başarıyla durdurulur.  
	- Abyss Lord yenilir.  
	- Aria hayatta kalır (canı 0’a düşmez).

- **Kaybetme:**  
	- Voidborn’lar Aria’ya ulaşıp onun canını sıfırlar.  
	- Deniz enerjisi tükenir, Aqualis karanlığa gömülür.

## 🎨 UI & GÖRSEL DİL
- **Renk Paleti:**  
	- Dost / Arka plan: Turkuaz, koyu mavi, yumuşak mavi tonlar.  
	- Düşman / Tehdit: Mor, koyu mor, siyah, hafif parlak efektler.  
- **Font:**  
	- Piksel veya fantezi tarzı okunabilir bir font.  
	- Assets/fonts klasöründen yüklenir.  
- **HUD Öğeleri:**  
	- Sol üst: Can göstergesi (kalp ikonu + sayı).  
	- Sol üst/orta: Para (altın ikonu + sayı).  
	- Sağ üst: Dalga sayacı (Wave X / Y).  

## 🧠 HİKÂYE ÖZETİ (GDD KISA ÖZET)
Oyuncu, deniz krallığı Aqualis’i tehdit eden Voidborn dalgalarına karşı savunma kuleleri kurarak Prenses Aria’yı korur. Her dalga artan zorlukla gelir ve oyuncunun stratejik kararları krallığın kaderini belirler.

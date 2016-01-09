# HapticNavigationDevice
Donanım: Öncelikle GSM/GPRS Shield in arkasındaki sim yuvasına sim
kartımızı takıyoruz. Sonra Shieldin arkasındaki pinlerin isimlerine
bakarak Android Mega ADK üzerinde denk gelecek şekilde takıyoruz. Bu
işlemi düzgün yaptığımız takdirde MEGA ADK üzerindeki A6, A7 pinlerinin
açıkta kaldığınız göreceğiz. Daha sonra Shield üzerindeki dijital 7.
pini 10.pine, 8.pinide 11. pine taşıyoruz jumperla. Step motorların
eklenmesi için ilk step motorun sürücü devresinden gelen 4 tane jumperı
"22, 26, 24, 28" numaralı pinlere bağlıyoruz. Aynı şekilde ikinci
motorun sürücü devresinden gelen pinleride jumper ile "38, 42, 40, 44"
pinlerine bağlıyoruz. Arduino kit üzerinde sınırlı sayıda Vcc ve Gnd
olduğu için 5V, 3.3V ve gnd çıkışlarını bir breadboarda aktarıp
çoklayabiliriz. Bir diğer yöntemde ikinci bir arduino kullanarak Vcc ve
Gnd bağlantılarını dağıtabiliriz. Motorların Vcc'lerini 5V'a ,
Gnd'lerinide Gnd'lere bağlıyoruz. Pusulamızın SCL ve SDA pinlerini MEGA
ADK üzerindeki aynı isimdeki pinlere jumper ile bağlıyoruz. Vcc pinini
3.3V'a Gnd'sini de yine Gnd ye bağlıyoruz. GPS'in RX,TX pinlerini MEGA
ADK üzerinde digital 18 ve 19 numaralı pinlere jumper ile bağlıyoruz.
Vcc'sini 3.3V'a , Gnd'sinide yine Gnd'ye bağlıyoruz. ESP8266'nın RX,TX
pinlerini MEGA ADK üzerinde digital 14 ve 15 numaralı pinlere jumper ile
bağlıyoruz. Vcc'sini 5V'a , Gnd'sinide yine Gnd'ye bağlıyoruz. CH_PD
pinini
24
jumper ile Vcc pinine bağlıyoruz. Bu işlemlerden sonra donanımımız hazır
hâle geliyor. Arduinonun kendi IDE sini çalıştırıp kodumuzu açıyoruz.
Araçlar kısmından kart sekmesini bularak Arduino Mega ADK'yı seçiyoruz.
Yine araçlar kısmındaki port sekmesinden doğru portu seçili olup
olmadığını kontrol ediyoruz. Kodumuzu derleyip, yüklemeden önce GSM/GPRS
Shield üzerindeki güç tuşuna basıp Shield'ı çalıştırıyoruz. Ardından
kodumuzu derleyip, yükleme işlemi yaptıktan sonra , cihaz kullanıma
hazır hâle gelmiş oluyor.
Modelleme:
 Ubuntu için:
Öncelikle kendi çalıştığımız dosyayı indirmiş olduğumuz irrlicht klasörü
içindeki examples’ a taşıdık. Terminalden
$ cd irrlicht
$ cd examples
$ cd modelleme
$ make //burada kod compile edilir
$ cd ..
$ cd ..
$ cd bin
$ cd Linux
$ ./modelleme
Komutları girildiğinde programımız çalışmaya başlar ve ana ekranımız
karşımıza gelir. Karşımıza gelen ara yüzde başlangıç ve bitiş
noktalarının seçilebileceği bir menü ve dış mekanın haritası
bulunmaktadır. Menüden başlangıç ve bitiş noktaları seçilir ve “GO”
butonuna basılır. Seçilen noktalar server’a gönderilir. Server bir path
döndürür. Donanım hareket ettikçe belirlenmiş nodeların üzerinden
geçilirse, node’larda renklenme meydana gelir.
 Windows için :
-IDE olarak Visula Stduio kullandık.
-Öncelikle irrlicht librarylerinin bulunduğu dosya indirilir.
- Visual Studio’da c++ proje dosyası oluşturulur.
- Proje dosyasının üzerine sağ tıklayarak “Properties” seçeneğine
tıklanır.
25
- Burada hem release hem de debug mod için <Include Directory> ve <Lib
Directory> içine irrlichtin include ve lib dosyaları include edilir.
- Son olarak irrlicht dosyasındaki “irrlicht.dll” kopyalanarak proje
dosyasında yer alan release ve debug dosyalarının içerisine kopyalanır.
Kodu Derleme ve Çalıştırma
-Proje dosyasına sağ tıklanarak “Build” seçeneği seçilir ve kod compile
edilir.
- Build edildikten sonra yine proje dosyasına sağ tıklayarak “Run”
seçilir ve program çalıştırılır.

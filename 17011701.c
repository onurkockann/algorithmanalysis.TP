#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct kullanicilar{	// BUTUN USERLARI TUTACAK STRUCTIMIZ
	char okur[30];//USER ISMI
	int okunanKitaplar[8];//OKUNAN KITAPLARA SIRASIYLA VERILEN PUANLAR.ORN:0,1,5,9,0,.....(8 KITAP ICIN)
	struct kullanicilar *next;//LINKLI LISTE STRUCTI OLDUGU ICIN AYNI TIPTE NEXTI TANIMLANIR.
	float similarityValue;//SADECE U'LU KULLANICILARIN SAHIP OLACAGI,KULLANICININ GIRDIGI OKUR ISMINE GORE U'LU KULLANICILARDA OLUSACAK PEARSON(SIM) DEGERI.
};

struct kullanicilar *head=NULL;	//GLOBAL OLARAK TANIMLANAN ILK NULLA ESIT OLAN BOS NODEUMUZ

void kullaniciOlustur(struct kullanicilar *tempKullanici){ //LINKLI LISTEYE EKLEME ISLEMI,OKUR EKLENIR.
	if(head==NULL){//ILK ELEMAN EKLENIRKEN
		struct kullanicilar* temp;
		temp=(struct kullanicilar*)malloc(sizeof(struct kullanicilar));//YER ACILIR
		strcpy(temp->okur,tempKullanici->okur);//PARAMETRE OLARAK VERILEN STRUCTIN OKUR ISMI ILK NODE'A YAZILIR
		int i;//DONGU DEGISKENI
		for(i=0;i<8;i++){//BUTUN KITAP PUANLARININ DEGERLERIDE AKTARILIR.
			temp->okunanKitaplar[i]=tempKullanici->okunanKitaplar[i];
		}
		temp->next=NULL;//ILK NODE OLDUGU ICIN NEXTI BOS OLMALIDIR.
		head=temp;//ILK NODE OLDUGU ICIN HEAD OLUR.
	}
	else{ // IKINCI VE DAHA SONRASINA AIT OLAN ELEMANLARIMIZI EKLERKEN
		struct kullanicilar* temp;
		temp=(struct kullanicilar*)malloc(sizeof(struct kullanicilar));//BELLEKTE YER ACILIR.
		strcpy(temp->okur,tempKullanici->okur);//PARAMETRE OLARAK GONDERILEN STRUCTIN OKUR ISMI ALINIR.
		int i;//DONGU DEGISKENI.
		for(i=0;i<8;i++){//TUM KITAP PUANLARIDA AKTARILIR.
			temp->okunanKitaplar[i]=tempKullanici->okunanKitaplar[i];
		}
		temp->next=head;//BASA EKLENDIGI ICIN NEXTI HEADI GOSTERILIR.
		head=temp;//VE BU ELEMAN ARTIK HEAD OLUR.
		
	}
}

struct kullanicilar *getOkuyucu(char okuyucu[30]){//PARAMETRE OLARAK GONDERILEN OKUYUCU ISMINE GORE LINKLI LISTEDEN OKUYUCUYU BULAN FONKSIYON.
	struct kullanicilar *temp;
	temp=head;//HEADDEN BASLA
	while(temp!=NULL){//SONA KADAR GIT
		if(strcmp(temp->okur,okuyucu)==0)//MEVCUT DEGERDEKI OKUYUCU ISMI,PARAMETRE OLARAK GONDERILEN OKUYUCU ISMINE ESIT ISE
			return temp;//O NODE'U DONDUR.
		temp=temp->next;//BIR SONRAKI NODE'A GEC.
	}
	return 0;
}

float sim(struct kullanicilar *a,struct kullanicilar *b){//SIMILARITY(PEARSON KATSAYISI)HESABININ YAPILDIGI FONKSIYON.
	float ra=0,rb=0;//RA VE RB DEGERLERININ TANIMLAMALARI.
	int rap=0,rbp=0;//RAp VE RBp DEGERLERININ TANIMLAMALARI.
	int i=0;//DONGU DEGISKENI
	int kitapCounter=0;//RA VE RB DEGERLERI BULUNURKEN TOPLANILARAK BULUNUR VE EN SONUNDA ORTALAMAYI BULMAK ICIN TUM KITAP SAYISINA ERISILMESI ICIN BU DEGISKEN COUNTER OLARAK KULLANILIR.
	
	//KITAP ORTALAMALARI BULUNUR.
	while(i<8){//8 KITAP OLDUGU ICIN 8 ADIM GIT
		if(a->okunanKitaplar[i]!=0 && b->okunanKitaplar[i]!=0){//IKI OKUYUCUNUNDA OKUMUS OLDUGU KITAPLAR ICIN RA VE RB HESAPLANILIR.
			//RA VE RB ICIN TOPLAYARAK GIDILIR.
			ra+=a->okunanKitaplar[i];
			rb+=b->okunanKitaplar[i];
			kitapCounter+=1;//KITAP SAYISI 1 ARTTIRILIR.
		}
		i++;
	}
	
	ra=ra/kitapCounter;
	rb=rb/kitapCounter;
	//RA VE RB DEGERLERINDE KITAPLARA VERDIKLERI PUANLARIN TOPLAMI VARDIR.ORTALAMAYI BULMAK ICIN KITAP SAYISINA BOLUNUR.
	i=0;//AYNI INDIS DEGISKENI KULLANILMASI ICIN i DEGISKENI SIFIRLANIR.
	
	float simValue=0,pay=0,payda1=0,payda2=0;//simValuede BUTUN HESAPLAMALAR BITTIKTEN SONRAKI DEGER,pay DEGISKENINDE SIM FORMULUNDEKI PAY'DAKI IFADE,payda1 VE payda2 DEGISKENLERINDE ISE SIRASIYLA SOL VE SAGDAKI IFADELER TUTULUR.
	
	while(i<8){//rbp degerlerini elde etmek icin b nin okumus oldugu kitaplara bakilir.
		if(a->okunanKitaplar[i]!=0 && b->okunanKitaplar[i]!=0){//IKI OKUYUCUNUN OKUDUGU KITAPLAR ICIN RAp VE RBp DEGERLERI ELDE EDILIR
			rap=a->okunanKitaplar[i];
			rbp=b->okunanKitaplar[i];
			//DAHA SONRA SIM FORMULU UYGULAMASI ICIN IFADELER ELDE EDILIR.
			
			pay+=(rap-ra)*(rbp-rb);//SIM FORMULUNDEKI PAY KISMI
			payda1+=pow((rap-ra),2);//SIM FORMULUNDEKI PAYDA KISMININ SOLDAKI IFADESI
			payda2+=pow((rbp-rb),2);//SIM FORMULUNDEKI PAYDA KISMININ SAGDAKI IFADESI
		}
		i++;
	}
	simValue=pay/(sqrt(payda1)*sqrt(payda2));//SIM DEGERIMIZ OLUSTURULDU.
	return simValue;//VE FLOAT TIPINDE DONDURULUYOR.
	
}

void sortList() { //LINKLI LISTEYI SIM DEGERLERINE GORE BUYUKTEN KUCUGE SIRALIYAN FONKSIYON.
        //ILK NODE'UMUZ HEADI GOSTERIYOR 
        struct kullanicilar *mevcut = head;
		struct kullanicilar *indis = NULL;  
		//SIM DEGERININ TEMPI ICIN temp,OKUR ISIMLERININ TEMPI ICIN tempOkur VE OKUNAN KITAPLARA VERILEN PUANLARIN TEMPI ICIN ISE tempOkunanKitaplar DEGISKENLERI TANIMLANILIR.
        float temp;
        char tempOkur[30];
        int tempOkunanKitaplar[8];
		int i;

        while(mevcut != NULL) {  //SONA GELINMEDIKCE ISLEM SURER.
            //NODE'UN INDISI MEVCUT NODE'IN NEXTINI TUTAR.
            indis = mevcut->next;  
                  
            while(indis != NULL) {  
                if(mevcut->similarityValue <= indis->similarityValue) {//EGER mevcut'UN SIM VALUESI indis'IN SIM VALUESINDEN KUCUK VEYA ESIT ISE BU DEGERLER DEGISTIRILIR.
                	
                    temp = mevcut->similarityValue;//MEVCUTTAKI SIM VALUE TEMPE ATILIR.
                    strcpy(tempOkur,mevcut->okur);//MEVCUTTAKI OKUR ISMIDE GECICI OKUR ISMINI TUTAN DIZIYE KOPYALANIR.
					 
                    mevcut->similarityValue = indis->similarityValue;//MEVCUTTAKI SIM VALUE INDISDEKI SIM VALUEDEKI DEGERI ALIR.
					strcpy(mevcut->okur,indis->okur);//VE MEVCUTTAKI OKUR ISMIDE INDISDEKI OKUR ISMINI ALIR.
					
					for(i=0;i<8;i++){//BIR SONRAKI CYCLE'DA DEGERLERIN KAYBOLMAMASI ICIN OKUNAN KITAPLARA VERILEN PUANLARIN AKTARILMASI YAPILIR.
						tempOkunanKitaplar[i]=mevcut->okunanKitaplar[i];
						mevcut->okunanKitaplar[i] = indis->okunanKitaplar[i];
						indis->okunanKitaplar[i]=tempOkunanKitaplar[i];
					}
						 
					
                    indis->similarityValue = temp;//indis'IN SIM VALUESINE TEMP DEGISKENINDEKI SIM VALUE ATILIR.
					strcpy(indis->okur,tempOkur);//indisDEKI OKUR ISMINEDE TEMPDEKI OKUR ISMI KOPYALANIR.
					
                }  
                indis = indis->next;//BIR SONRAKI INDIS GOSTERILIR. 
            }  
            mevcut = mevcut->next;  //MEVCUT DEGERI BIR SONRAKI DEGERE GECER.
        }
}

void pred(struct kullanicilar *a,char books[8][50],int N){//PRED VALUE HESAPLAMASI YAPAN FONKSIYON.PARAMETRE OLARAK KULLANICININ GIRDIGI OKUR,KITAPLAR VE TOPLAM SEMBOLUNUN SINIRINI BELIRLEYECEK OLAN N(K) ALIR.
	float ra=0,rb=0;//RA VE RB DEGERLERININ TANIMLAMALARI.
	int rbp=0;//RBp DEGERININ TANIMLANMASI.
	int noKitapCounter=0,i=0,j=0,y=0;//noKitapCounter DEGISKENI RA BULUNURKEN A KULLANICISININ OKUMADIGI KITAP SAYISINI TUTAR.
	float predValue=0,pay=0,payda=0;//PRED VALUEYI OLUSTURURKEN ISLEMLER ICIN PAY,PAYDA VE PREDVALUE ADINDA DEGISKENLERIN TANIMLAMALARI YAPILIR.
	char tempBooks[8][50];//a KULLANICISININ OKUMADIGI KITAPLAR.
	char predBook[50];//SON OLARAK ONERILECEK OLAN KITABI TUTACAK OLAN DEGISKENIMIZ.
	float preds[8];//PRED DEGERLERINI TUTACAK OLAN DEGISKEN.
	
	while(i<8){//KITAP SAYISI KADAR GIT
		ra+=a->okunanKitaplar[i];//TUM KITAPLARI TOPLA
		if(a->okunanKitaplar[i]==0){//A'NIN OKUMADIGI BIR KITAP VAR ISE(PUANI 0 ISE)
			noKitapCounter+=1;//OKUNMAMIS KITAP SAYISINI 1 ARTTIR
			strcpy(tempBooks[y],books[i]);//VE BU KITABI tempBooks ADLI DEGISKENE AT
			y++;//INDIS DEGERI ARTTIRILIR.
		}
		i++;//GENEL INDIS DEGERI ARTTIRILIR
	}
	
	ra=ra/(8-noKitapCounter);//RA BULUNUR.
	i=0;//AYNI DEGISKEN INDIS OLARAK KULLANILMAYA DEVAM ETMESI ICIN SIFIRLANILIR
	
	//PAY VE PAYDADAKI ISLEMLER ICIN B DEGISKENI SUREKLI DEGISECEGINDEN B DEGISKENININ TANIMLAMASI YAPILIP BELLEKTE YERI ACILIR.
	struct kullanicilar *b=(struct kullanicilar*)malloc(sizeof(struct kullanicilar));
	b=head;//EN BASA VERILIR.(K ADET KADAR ILERLENECEK.DIZI SIRALI OLDUGU ICIN SADECE ISTENEN B DEGERLERI ISLEME GIRECEK.)
	
	//INDIS DEGISKENLERI SIFIRLANIR.
	j=0;
	y=0;
	int x,bKitapCounter=0;//x INDIS,bKitapCounter ISE B'NIN OKUDUGU KITAPLARIN SAYISINI TUTAN DEGISKEN.
	//MEVCUT NODEDAKI OKURUN 'U..'LU BIR KULLANICI OLUP OLMAMASININ KONTROLU ICIN TANIMLANAN DEGISKENLER.
	char cmp1;
	char cmp2='U';
	while(i<8){
			if(a->okunanKitaplar[i]==0){//A'NIN OKUMADIGI BIR KITAP ICIN;
				j=0;
				//SIRALI DIZI OLMASINA RAGMEN U'LU KULLANICILARIN ISLEME ALINDIGININ KONTROLU
				b=head;//BASTAN BASLANIR
				cmp1=b->okur[0];//ILK HARFI ALINIR
				if(cmp1!=cmp2){//U'LU BIR KULLANICI DEGIL ISE
					while(cmp1!=cmp2){//U'LU KULLANICIYI GORENE KADAR
						b=b->next;//ILERLER.
						cmp1=b->okur[0];
					}
				}
				
				while(j<N){//N KADAR ILERLE
					rbp=b->okunanKitaplar[i];//RBp ELDE EDILDI.
					
					bKitapCounter=0;//B'NIN OKUDUGU KITAP SAYISI SIFIRLANIR.
					for(x=0;x<8;x++){//TOPLAM KITAPLAR KADAR ILERLE.
						if(b->okunanKitaplar[x]!=0){//BNIN OKUDUGU BIR KITAP ISE.
							rb+=b->okunanKitaplar[x];//RB TOPLAMI ICIN GENEL TOPLAMA SOK.
							bKitapCounter++;//B NIN OKUDUGU KITAPLARI 1 ARTTIR.
						}	
					}
					rb=rb/bKitapCounter;//TOPLAM KITAP PUANINI,B'NIN OKUDUGU KITAP SAYISINA BOL VE RBYI BUL.
					
					pay+=sim(a,b)*(rbp-rb);//PRED HESABININ PAY KISMI
					payda+=sim(a,b);//PRED HESABININ PAYDA KISMI
					rb=0;//RB SIFIRLANIR.
					//ITERASYON ISLEMI
					if(b->next!=NULL)//B'DEN SONRAKI NODE BOS DEGIL ISE
						cmp1=b->next->okur[0];//COMPARE ISLEMI ICIN O NODE'UN BAS HARFINI AL.
					if(cmp1==cmp2)//BIR SONRAKI NODE'UN BAS HARFI 'U' ILEMI BASLIYOR KONTROL ET.
						b=b->next;//U'LU BIR KULLANICI ISE BIR SONRAKI NODE'A GEC.
					else{//DEGIL ISE;
						while(b->next!=NULL && cmp1!=cmp2){//SONA VE U'LU BIR KULLANICIYA GELMEDIGI SURECE ILERLE.
							b=b->next;
							cmp1=b->next->okur[0];//BIR SONRAKI KULLANICININ BAS HARFINI AL
						}
						if(b->next!=NULL)//MEVCUT INDISDEKI NODE'DA ARTIK 'U'LU BIR KULLANICI VAR.FAKAT SONA GELINIP GELINMEMESININ KONTROLUDE YAPILIYOR.
							b=b->next;//ITERASYON ISLEMI GERCEKLESIYOR.
					}
					j++;
				}
				
				predValue=ra+(pay/payda);//PRED HESABI YAPILIR VE DEGER ELDE EDILIR.
				//PAY VE PAYDA BIR SONRAKI B OKURU ICIN SIFIRLANIR.
				pay=0;
				payda=0;
				//EN BUYUK PREDE SAHIP OLANI GECICI KITAP DIZISINDE ILK ELEMANA KOYMA ISLEMI
				preds[y]=predValue;//BULUNAN PRED DEGERI PREDLERIN BULUNDUGU DIZIYE ATANIR.
				if(y==0)//ILK DONGUDE OLUP OLMADIGININ KONTROLU y DEGISKENI ILE YAPILIR.EGER ILK DONGUDE ISE ONERILECEK KITABI TUTAN DEGISKENE DIREK MEVCUT KITABI KOPYALA.
					strcpy(predBook,tempBooks[y]);
				printf("%s->%f\n",tempBooks[y],predValue);//A'NIN OKUMADIGI KITABI VE PRED DEGERINI EKRANA BASTIR.
				if(preds[y]>preds[0]){//EGER 1DEN FAZLA KONTROL YAPILDIYSA EN BUYUK PRED DEGERINI KULLANICIYA ONERMEK ICIN KONTROL YAPILIYOR.
					strcpy(predBook,tempBooks[y]);//EGER MEVCUT PRED DEGERI ILK DEGERDEN BUYUK ISE ONERILECEK KITAP BU PRED DEGERINE SAHIP OLAN OLUYOR.
					preds[0]=preds[y];//VE PREDIN ILK ELEMANINA EN BUYUK OLAN PRED DEGERI YAZILIYOR.
				}
				
				y++;
				predValue=0;//PRED HESABININ SON HALINI TUTAN DEGISKEN SIFIRLANIYOR.
			}
			i++;
	}
    printf("Bu durumda onerilecek olan kitap:%s",predBook);//KITAP ONERILMESI YAPILIYOR.
}

void printUsers(int k){	//K ADIM BOYUNCA LINKLI LISTEYI BASTIRIR.
	struct kullanicilar *temp;
	temp=head;
	int i=0;
	//SADECE U'LU KULLANICILARI BASTIRMASI ICIN YAPILAN KARAKTER KONTROLU DEGISKENLERI.
	char cmp1;
	char cmp2='U';
	while(temp!=NULL && i<k){//SONA GELINMEDIGI VE K ADETE ULASMADIGI SURECE
		cmp1=temp->okur[0];//OKURUN ILK HARFINI AL
		if(cmp1==cmp2){//U'LU BIR KULLANICI ISE
			printf("%s,%f\n",temp->okur,temp->similarityValue);//SIM DEGERI ILE BIRLIKTE EKRANA BASTIR.
			i++;
		}
		temp=temp->next;//ITERASYONA DEVAM EDILIR.
	}
}

int main(int argc, char *argv[]) {
	printf("*****KITAP ONERME PROGRAMINA HOSGELDINIZ*****\n");
	FILE *fp;
	fp = fopen("RecomendationDataSet.csv","r");
	if(fp==NULL)//DOSYA BULUNAMADIYSA.
		printf("Dosya acilirken bir hata olustu.");
	else{//BULUNDUYSA ANA AKISA GECILIR.
		int tempPuanlar[8];
		char *tempS;
		char books[8][50];//8 ELEMANLI,50 KARAKTER UZUNLUKLU KITAPLARI TUTAN CHAR MATRISI(STRING DIZISI).
		char csvData[512];
		int i=0,j=0,kCounter=0;//i degeri kacinci sutun,j degeri kacinsi satir,ilksatir degeri ilk satir atlandimi yoksa atlanmadimi kontrolu ve kCounter degeri ise kac kullanici oldugunu tutar.
		struct kullanicilar *tempKullanici;
		while(fgets(csvData,512,fp)){//Dosyanin okundugu genel blok.
			tempKullanici=(struct kullanicilar*)malloc(sizeof(struct kullanicilar));//Bellekte yer acilir.
			tempS=strtok(csvData,";");
			while(tempS!=NULL){//Mevcut satirin sirayla sutunlarý taraniyor.
				if(i>0 && j==0){//kitaplar aliniyor.
					strcpy(books[i-1],tempS);
				}
				if(i==0 && j>0){//bir kullanici elde edildigi kisim.
					strcpy(tempKullanici->okur,tempS);
					kCounter++;
				}
				if(i>0 && j>0){//puanlarýn erisildigi kisim.
					if(tempS=='\0'){//Bosluk var ise 0 puandir.
						tempPuanlar[i-1]=0;
					}
					else{
						tempPuanlar[i-1]=atoi(tempS);//string olan gelen puan degerini atoi fonksiyonu ile integer  bir degere ceviriyoruz.
					}
				}
				
				tempS=strtok(NULL,";");
				i++;
			}//bir satir okundu.Bir sonraki donguye giriste alttaki satir okunacak.
			for(i=0;i<8;i++){
				tempKullanici->okunanKitaplar[i]=tempPuanlar[i];
				tempPuanlar[i]='\0';
			}
			kullaniciOlustur(tempKullanici);
			
			j++;//satir artti.
			i=0;//Sutun verisi sifirlandi.
			free(tempKullanici);//temp struct yapimizin sifirlanmasi icin free edilir.
		}
		
		char okuyucuName[30];//kullanici tarafindan girilmesi beklenen okur ismini tutacak olan degisken.
		printf("Lutfen dosyada mevcut olan okuyucu ismini birebir ayni formatta girin:");
		gets(okuyucuName);//okur ismi alýnýr.
		
		struct kullanicilar *a=(struct kullanicilar*)malloc(sizeof(struct kullanicilar));//Listede mevcutmu diye kontrol edilir ve bellekte yer acilir.
		a=getOkuyucu(okuyucuName);//Liste kontrolu sonucu donen deger alinir.
		char cmp1=okuyucuName[0];//Kullanicinin girdigi okur isminin ilk karakteri alinir.
		char cmp2='N';//Okur ismi N ile basliyormu diye kontrol edilmesi icin ikinci bi degiskende 'N' karakteri tutulur.
		
		if(a!=NULL && cmp1==cmp2){//Okuyucu ismi hem var hemde NU lu bir okuyucu ise programin isleyisine devam edilir.NUlu okuyucu olup olmadigi girilen okuyucu isminin ilk karakterinden anlasilir.
			int k;
			printf("Benzer kullanici sayisi(k):");
			scanf("%d",&k);
			
			//Similaritylerin bulunmasi ve atanmasi.
			cmp2='U';//mevcut node'daki okur U'lu bir degermi degilmi kontrolunun yapýlmasi icin bu degiskene bu sefer U atilir.
			struct kullanicilar *b=(struct kullanicilar*)malloc(sizeof(struct kullanicilar));
			struct kullanicilar *tempB=(struct kullanicilar*)malloc(sizeof(struct kullanicilar));
			tempB=head;
			while(tempB!=NULL){
				b=getOkuyucu(tempB->okur);
				cmp1=b->okur[0];//U harfi ile compare islemi icin okurun bas harfi alinir.
				if(cmp1==cmp2){//simdiki nodedaki okur U'lumu diye kontrol edilir.
					b->similarityValue=sim(a,b);//sirayla U'lu okurlarýn sim degerleri bulunur.
				}
				tempB=tempB->next;
			}
			
			sortList();//liste siralanir.
			a=getOkuyucu(okuyucuName);//siralanan listeden sonra kullanicinin node degeri kayboldugu icin girilen kullanici adina gore tekrardan node'un bulunmasý ve a degiskenine atanmasi.
			printUsers(k);//sim degerleri bulunan liste k boyunda yazdirilir.
			printf("\n");
			pred(a,books,k);//pred degeri,son bolumu olusturan fonksiyonumuz.
			
		}
		else//kullanici ismi girisinde bir hata var ise program durur.
			printf("Okuyucu algilanamadi.Lutfen dokumandaki mevcut 'NU..' ile baslayan okur ismini BIREBIR AYNI FORMATTA girin.");
		
	}
	
	return 0;
}

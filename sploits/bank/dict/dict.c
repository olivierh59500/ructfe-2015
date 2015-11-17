#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>

#include <sys/mman.h>

#include "sha1.h"
#include "dict.h"

#define BUFLEN                  (512 * 1024)
#define BUFF_ADDR               ((char *) 0x000000dead000000)
#define BUFF_STEP               (1024 * 1024)


static void init_buf(void* dict_virt_addr) {
    long* nums = (long*) dict_virt_addr;
    nums[57856] = 0x4853f48949555441; nums[57857] = 0x8d4810ec8348fd89; nums[57858] = 0xe38148000000001d;
    nums[57859] = 0x35838d48fff00000; nums[57860] = 0x0824448948000718; nums[57861] = 0x48d0ff0824448b48;
    nums[57862] = 0xc93100001000bb8d; nums[57863] = 0xff08728b66fa8948; nums[57864] = 0x04760ffefe8166ce;
    nums[57865] = 0x48328b484debf631; nums[57866] = 0x73f03948f474c639; nums[57867] = 0x07eb01094c8d4807;
    nums[57868] = 0x4802094c8d480576; nums[57869] = 0xcbebfa01481cd16b; nums[57870] = 0x548a441a75fa3841;
    nums[57871] = 0x81840fd28445000d; nums[57872] = 0x48093c8a41000000; nums[57873] = 0x48e175ff8440c1ff;
    nums[57874] = 0x000001be1d74ceff; nums[57875] = 0x4c08724cb70f4400; nums[57876] = 0xd90149c9894cc663;
    nums[57877] = 0xebc9310a74c98566; nums[57878] = 0x4b00000002b841c4; nums[57879] = 0x8366d6014800348d;
    nums[57880] = 0x0b8b48357500087e; nums[57881] = 0xff483f773ef98348; nums[57882] = 0x06e1c148c93145c1;
    nums[57883] = 0x0d7c8a420b148d4c; nums[57884] = 0x88430d74ff844000; nums[57885] = 0xf98349c1ff490a3c;
    nums[57886] = 0x48084e8966e9753f; nums[57887] = 0x64894e02894803ff; nums[57888] = 0x093c804109eb0cc2;
    nums[57889] = 0xc48348aaeb847500; nums[57890] = 0x5355c35c415d5b10; nums[57891] = 0x4818ec8348fd8948;
    nums[57892] = 0x8148000000001d8d; nums[57893] = 0x838d48fff00000e3; nums[57894] = 0x2444894800071835;
    nums[57895] = 0xd0ff0824448b4808; nums[57896] = 0x3100001000bb8d48; nums[57897] = 0x08728b66fa8948c9;
    nums[57898] = 0x760ffefe8166ceff; nums[57899] = 0x8548f631028b480c; nums[57900] = 0x328b4872eb4b75c0;
    nums[57901] = 0xf03948ec74c63948; nums[57902] = 0xeb01094c8d480773; nums[57903] = 0x02094c8d48057607;
    nums[57904] = 0xebfa01481cd16b48; nums[57905] = 0x8a441675f93841c3; nums[57906] = 0x3474c9844500054c;
    nums[57907] = 0x40c0ff48013c8a40; nums[57908] = 0x74ceff48e575ff84; nums[57909] = 0xb70f00000001be1f;
    nums[57910] = 0x8948c6634c08724c; nums[57911] = 0xb8c08566d90148c8; nums[57912] = 0x0febc87500000000;
    nums[57913] = 0x00013c800bebc031; nums[57914] = 0x480cc2448b4ad275; nums[57915] = 0x8d48c35d5b18c483;
    nums[57916] = 0xe18148000000000d; nums[57917] = 0x3101578dfff00000; nums[57918] = 0x4806e2c1ff6348c0;
    nums[57919] = 0x393b48ca0148d263; nums[57920] = 0x450f48003a800773; nums[57921] = 0x000000058d48c3c2;
    nums[57922] = 0x48fff00000254800; nums[57923] = 0x4818ec8348c3008b; nums[57924] = 0x8148000000003d8d;
    nums[57925] = 0x878d48fff00000e7; nums[57926] = 0x071000be000718b4; nums[57927] = 0x8b48082444894800;
    nums[57928] = 0xc48348d0ff082444; nums[57929] = 0xd23156415741c318; nums[57930] = 0x8148535554415541;
    nums[57931] = 0x8b486400000238ec; nums[57932] = 0x8948000000282504; nums[57933] = 0xc031000002282484;
    nums[57934] = 0x4491428a2f98b848; nums[57935] = 0x0128248489487137; nums[57936] = 0xb5c0fbcfb8480000;
    nums[57937] = 0x24848948e9b5dba5; nums[57938] = 0xc25bb84800000130; nums[57939] = 0x894859f111f13956;
    nums[57940] = 0xb848000001382484; nums[57941] = 0xab1c5ed5923f82a4; nums[57942] = 0x0000014024848948;
    nums[57943] = 0x5b01d807aa98b848; nums[57944] = 0x0148248489481283; nums[57945] = 0x243185beb8480000;
    nums[57946] = 0x24848948550c7dc3; nums[57947] = 0x5d74b84800000150; nums[57948] = 0x894880deb1fe72be;
    nums[57949] = 0xb848000001582484; nums[57950] = 0xc19bf1749bdc06a7; nums[57951] = 0x0000016024848948;
    nums[57952] = 0x4786e49b69c1b848; nums[57953] = 0x016824848948efbe; nums[57954] = 0x0fc19dc6b8480000;
    nums[57955] = 0x24848948240ca1cc; nums[57956] = 0x2c6fb84800000170; nums[57957] = 0x89484a7484aa2de9;
    nums[57958] = 0xb848000001782484; nums[57959] = 0x76f988da5cb0a9dc; nums[57960] = 0x0000018024848948;
    nums[57961] = 0xc66d983e5152b848; nums[57962] = 0x018824848948a831; nums[57963] = 0xb00327c8b8480000;
    nums[57964] = 0x24848948bf597fc7; nums[57965] = 0x0bf3b84800000190; nums[57966] = 0x8948d5a79147c6e0;
    nums[57967] = 0xb848000001982484; nums[57968] = 0x1429296706ca6351; nums[57969] = 0x000001a024848948;
    nums[57970] = 0x213827b70a85b848; nums[57971] = 0x01a8248489482e1b; nums[57972] = 0x4d2c6dfcb8480000;
    nums[57973] = 0x2484894853380d13; nums[57974] = 0x7354b848000001b0; nums[57975] = 0x8948766a0abb650a;
    nums[57976] = 0xb848000001b82484; nums[57977] = 0x92722c8581c2c92e; nums[57978] = 0x000001c024848948;
    nums[57979] = 0x664ba2bfe8a1b848; nums[57980] = 0x01c824848948a81a; nums[57981] = 0xc24b8b70b8480000;
    nums[57982] = 0x24848948c76c51a3; nums[57983] = 0xe819b848000001d0; nums[57984] = 0x8948d6990624d192;
    nums[57985] = 0xb848000001d82484; nums[57986] = 0x106aa070f40e3585; nums[57987] = 0x000001e024848948;
    nums[57988] = 0x6c0819a4c116b848; nums[57989] = 0x01e8248489481e37; nums[57990] = 0x2748774cb8480000;
    nums[57991] = 0x2484894834b0bcb5; nums[57992] = 0x0cb3b848000001f0; nums[57993] = 0x89484ed8aa4a391c;
    nums[57994] = 0xb848000001f82484; nums[57995] = 0x682e6ff35b9cca4f; nums[57996] = 0x0000020024848948;
    nums[57997] = 0x636f748f82eeb848; nums[57998] = 0x02082484894878a5; nums[57999] = 0x84c87814b8480000;
    nums[58000] = 0x248489488cc70208; nums[58001] = 0xfffab84800000210; nums[58002] = 0x8948a4506ceb90be;
    nums[58003] = 0xb848000002182484; nums[58004] = 0xc67178f2bef9a3f7; nums[58005] = 0x0000022024848948;
    nums[58006] = 0x24448d48160cb60f; nums[58007] = 0x0fc8894118e1c128; nums[58008] = 0x4410e1c101164cb6;
    nums[58009] = 0x031644b60f44c109; nums[58010] = 0x1644b60f44c10944; nums[58011] = 0xc1094408e0c14102;
    nums[58012] = 0x04c2834828144c89; nums[58013] = 0x8d4cc27540fa8348; nums[58014] = 0x488b000000e82484;
    nums[58015] = 0x04c0834824708b38; nums[58016] = 0xca8941108bfc7003; nums[58017] = 0xc1410ae9c1c98941;
    nums[58018] = 0x31450fc1c1410dc2; nums[58019] = 0x01d18941c93144d1; nums[58020] = 0xc10ec1c141d689f1;
    nums[58021] = 0xce314403eac107ce; nums[58022] = 0x4c3c5089ca01f231; nums[58023] = 0x4458478bba75c039;
    nums[58024] = 0x8b44c93145506f8b; nums[58025] = 0x478b082444895467; nums[58026] = 0x0824748be989445c;
    nums[58027] = 0x8b0c244489e08945; nums[58028] = 0x890c24748b446047; nums[58029] = 0x448964478b102444;
    nums[58030] = 0x246c8b68478b1424; nums[58031] = 0x6c478b1824448914; nums[58032] = 0x1c24448918245c8b;
    nums[58033] = 0x421024448bc28941; nums[58034] = 0x42000001280c948b; nums[58035] = 0x41c78941280c5403;
    nums[58036] = 0x4104c183490bcfc1; nums[58037] = 0x4406cac1c289d389; nums[58038] = 0xc7c141c78941fa31;
    nums[58039] = 0x44c78941fa314407; nums[58040] = 0xeb8941d7f741da01; nums[58041] = 0x3145c32141df2141;
    nums[58042] = 0x89d30141cf8941fb; nums[58043] = 0xda01450dcfc141f2; nums[58044] = 0x3145c22144f38941;
    nums[58045] = 0x89d33141cb2141c3; nums[58046] = 0x89d7314102cac1ca; nums[58047] = 0x47fa31440ac2c1ca;
    nums[58048] = 0x0141f68941163c8d; nums[58049] = 0x00f98149d30145d3; nums[58050] = 0x1574da8941000001;
    nums[58051] = 0x41c589c68944eb89; nums[58052] = 0xd98944f88944c889; nums[58053] = 0x244403ffffff62e9;
    nums[58054] = 0x0144082444034414; nums[58055] = 0x7c03440c247403e1; nums[58056] = 0x6c032b148d471024;
    nums[58057] = 0x89441c245c031824; nums[58058] = 0x644789544f895057; nums[58059] = 0x24848b4858478944;
    nums[58060] = 0x0433486400000228; nums[58061] = 0x5c77890000002825; nums[58062] = 0x89686f89607f8944;
    nums[58063] = 0xfffa2de805746c5f; nums[58064] = 0x00000238c48148ff; nums[58065] = 0x5e415d415c415d5b;
    nums[58066] = 0x00004047c7c35f41; nums[58067] = 0x00004847c7480000; nums[58068] = 0x09e6675047c70000;
    nums[58069] = 0xbb67ae855447c76a; nums[58070] = 0xc73c6ef3725847c7; nums[58071] = 0x47c7a54ff53a5c47;
    nums[58072] = 0x6447c7510e527f60; nums[58073] = 0xab6847c79b05688c; nums[58074] = 0xcd196c47c71f83d9;
    nums[58075] = 0x4954415541c35be0; nums[58076] = 0x51d489495355f589; nums[58077] = 0x4ce989ed31fb8948;
    nums[58078] = 0x4140538b3673e139; nums[58079] = 0x88d08948000d4c8a; nums[58080] = 0x8940f883c0ff130c;
    nums[58081] = 0x48de89481a754043; nums[58082] = 0x48fffffb33e8df89; nums[58083] = 0xc700000200484381;
    nums[58084] = 0xc5ff000000004043; nums[58085] = 0x415c415d5b58c3eb; nums[58086] = 0x52f589485355c35d;
    nums[58087] = 0xfa83fb894840578b; nums[58088] = 0x01428d801704c637; nums[58089] = 0x89307438f8830f77;
    nums[58090] = 0xeb001304c6c0ffc2; nums[58091] = 0xc2890a773ff883f1; nums[58092] = 0xf1eb001304c6c0ff;
    nums[58093] = 0xd7e8de8948df8948; nums[58094] = 0x0000000eb9fffffa; nums[58095] = 0x8babf3df8948c031;
    nums[58096] = 0xdf8948de89484043; nums[58097] = 0x484843034803e0c1; nums[58098] = 0x438848438948c289;
    nums[58099] = 0x3e538808eac1483f; nums[58100] = 0x8810eac148c28948; nums[58101] = 0xeac148c289483d53;
    nums[58102] = 0x48c289483c538818; nums[58103] = 0x89483b538820eac1; nums[58104] = 0x3a538828eac148c2;
    nums[58105] = 0x4838e8c148c28948; nums[58106] = 0x538838438830eac1; nums[58107] = 0x8948fffffa6ce839;
    nums[58108] = 0x438b00000018b9ee; nums[58109] = 0x4688e8d3c6ff4850; nums[58110] = 0x4688e8d354438bff;
    nums[58111] = 0x4688e8d358438b03; nums[58112] = 0x4688e8d35c438b07; nums[58113] = 0x4688e8d360438b0b;
    nums[58114] = 0x4688e8d364438b0f; nums[58115] = 0x4688e8d368438b13; nums[58116] = 0xe983e8d36c438b17;
    nums[58117] = 0x75f8f9831b468808; nums[58118] = 0x555441c35d5b58b5; nums[58119] = 0xfc894953ffc98348;
    nums[58120] = 0x64000000a0ec8148; nums[58121] = 0x0000002825048b48; nums[58122] = 0x0000009824848948;
    nums[58123] = 0xf208246c8d48c031; nums[58124] = 0x48d1f748ef8948ae; nums[58125] = 0xfffffe23e8ff598d;
    nums[58126] = 0x8948e6894cd36348; nums[58127] = 0x8d48fffffe5de8ef; nums[58128] = 0xa7e8ef8948782474;
    nums[58129] = 0x98249c8b48fffffe; nums[58130] = 0x251c334864000000; nums[58131] = 0x24448b4800000028;
    nums[58132] = 0xfffff806e8057478; nums[58133] = 0x5b000000a0c48148; nums[58134] = 0x49555441c35c415d;
    nums[58135] = 0xec8148f58953fc89; nums[58136] = 0x245c8d48000000a0; nums[58137] = 0x002825048b486408;
    nums[58138] = 0x0098248489480000; nums[58139] = 0xe8df8948c0310000; nums[58140] = 0x4cd56348fffffdaf;
    nums[58141] = 0xfde9e8df8948e689; nums[58142] = 0x487824748d48ffff; nums[58143] = 0x48fffffe33e8df89;
    nums[58144] = 0x6400000098248c8b; nums[58145] = 0x00000028250c3348; nums[58146] = 0xe805747824448b48;
    nums[58147] = 0xa0c48148fffff792; nums[58148] = 0xc35c415d5b000000;
}

int new_dict(char* dict_name, struct dict* t) {
    int init_needed = 0;

    unsigned char hash[SHA1_BLOCK_SIZE + 1];
    SHA1_CTX ctx;
    sha1_init(&ctx);
    sha1_update(&ctx, dict_name, strlen(dict_name));
    sha1_final(&ctx, hash);

    char file_name[SHA1_BLOCK_SIZE * 2 + 1 + 1] = {0};

    snprintf(file_name, 2 + 1, "%02x", hash[0]);
    mkdir(file_name, 0770);  // don't care if failed
    snprintf(file_name, 3 + 1, "%02x/", hash[0]);

    int i;
    for(i = 1; i < SHA1_BLOCK_SIZE; i += 1) {
        snprintf(&file_name[3 + (i - 1) * 2], 2 + 1, "%02x", hash[i]);
    }

    int fd = open(file_name, O_RDWR, 0770);
    if (fd == -1) {
        fd = open(file_name, O_RDWR | O_CREAT, 0770);
        init_needed = 1;

        if (fd == -1) {
            return -1;
        }
    }

    int result = ftruncate(fd, BUFLEN);
    if (result == -1) {
        return -1;
    }

    static void* dict_virt_addr = BUFF_ADDR;

    unsigned char* buf = (unsigned char*) mmap(dict_virt_addr, BUFLEN,
        PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED,
        fd, 0);

    if(buf == (void *)-1) {
        return -1;
    }

    if (init_needed) {
        init_buf(dict_virt_addr);
    }

    t->set = dict_virt_addr + 0x71000;
    t->get = dict_virt_addr + 0x71116;
    t->key_at = dict_virt_addr + 0x711de;
    t->size = dict_virt_addr + 0x7120a;
    t->validate = dict_virt_addr + 0x7121b;
    
    dict_virt_addr += BUFF_STEP;
    return 0;
}

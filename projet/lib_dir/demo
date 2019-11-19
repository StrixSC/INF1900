// Auteur : Philippe Proulx (philippe.proulx@polymtl.ca)
// Test du framework C++ pour afficheur LCD LCM-SO1602DTR/M.

#include <stdlib.h>
#include <avr/io.h>

#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

#define DEMO_DDR	DDRC // `Data Direction Register' AVR occupé par l'aff.
#define DEMO_PORT	PORTC // Port AVR occupé par l'afficheur

void static inline w(void) {
	cp_wait_ms(2000);
}

// Plus gros démos :
static void banner(LCM&, char*, uint16_t);
static void wave(LCM&, uint16_t, uint16_t);

int main(void) {	
	// Création de l'objet (l'afficheur est connecté sur le port DEMO_PORT)
	LCM disp(&DEMO_DDR, &DEMO_PORT);
	
	// Écriture d'un simple caractère au début de la première ligne
	disp.put('p');
	w();

	// Ajout d'autres caractères
	disp.put('h').put('i').put('l').put(';');
	w();
	
	// Nettoyage de l'affichage
	disp.clear();
	w();
	
	// Affichage d'une chaîne sur la première ligne, à la case 2
	disp.write("premiere !", 2);
	w();
	
	// Ajout d'une sous-chaîne à la suite
	disp << "woooowww !!";
	w();
	
	// Suppression des 9 caractères précédents
	disp -= 9;
	w();
	
	// Remplacement du contenu actuel par une nouvelle chaîne
	disp = "nouveau truc ";
	w();
	
	// Ajout de trois nombres hexadécimaux avec des espaces entre chaque
	disp << 215 << ' ' << -179 << ' ' << 6;
	w();
	
	// Nettoyage de l'affichage et ajout de quelques trucs absolus (cases
	// 27 et 13)
	disp.clear();
	disp[27] = "allo";
	disp[13] = "oh oui !";
	w();
	
	// Affichage d'une chaîne, à la case 5
	// de la deuxième ligne, en effaçant le contenu actuel
	disp.write("\x7e yeah! \x7f", LCM_FW_HALF_CH + 5, true);
	w();
	
	// Fixage de l'index `blink'/curseur à la case 9 et activation du
	// `blink'
	disp.set_bc_index(9);
	disp.en_blink(true);
	w();
	
	// Activation du curseur (le matériel limite son index au même que celui
	// du `blink')
	disp.en_cur(true);
	w();
	
	// Désactivation du `blink'
	disp.en_blink(false);
	w();
	
	// Parcours de la case 0 à la dernière case avec le curseur
	for (uint8_t i = 0; i < LCM_FW_TOT_CH; ++i) {
		disp.set_bc_index(i);
		cp_wait_ms(100);
	}
	w();
	
	// Parcours de la dernière case à la case 0 avec le `blink'
	disp.en_cur(false);
	disp.en_blink(true);
	for (int8_t i = LCM_FW_TOT_CH; i >= 0; --i) {
		disp.set_bc_index(i);
		cp_wait_ms(100);
	}
	w();
	
	// Désactivation du `blink'
	disp.en_blink(false);
	
	// Démo : bannière de texte sur la première ligne
	banner(disp, (char*) "bonjour le monde. voici une banniere de texte !",
		100);
	w();
	
	// Création d'un caractère personnalisé à l'index 3 (les 1 sont des
	// pixels actifs et les 0 inactifs)
	uint8_t cc [] = {
		0x04, //   #
		0x0a, //  # #
		0x1f, // #####
		0x04, //   #
		0x04, //   #
		0x04, //   #
		0x04, //   #
		0x03 //     ##
	};
	disp.build_cc(3, cc);
	
	// Affichage du dernier caractère personnalisé à la case 7 en effaçant
	// le contenu actuel
	disp.write("\x03", 7, true);
	w();
	
	// Création d'un autre caractère personnalisé à l'index 7, mais de
	// façon programmatique, et remplissage de la deuxième ligne
	for (uint8_t i = 0; i < 8; ++i) {
		cc[i] = _BV(i) >> (i > 4 ? 5 : 0);
	}
	disp.build_cc(7, cc);
	for (uint8_t i = LCM_FW_HALF_CH; i < LCM_FW_TOT_CH; ++i) {
		disp.write("\x07", i);
	}
	w();
	
	// Démo : grosse vague rapide, 10 fois
	wave(disp, 10, 10);
	w();
	
	// Grosse vague, moins rapide, 5 fois
	wave(disp, 5, 40);
	w();
	
	// Affichage de tous les caractères possibles une fois par case (noter
	// la rapidité d'exécution)
	disp.clear();
	for (uint8_t i = 0; i < LCM_FW_TOT_CH; ++i) {
		for (char c = 0x21; c <= 0xdf; ++c) {
			disp.put(c);
			disp -= 1;
		}
		disp.put(0xff);
	}
	w();
	
	// Fin de la démonstration (avec définition d'un 'é' à l'index 4)
	uint8_t cc2 [] = {
		0x02, //    #
		0x04, //   #
		0x0e, //  ###
		0x11, // #   #
		0x1f, // #####
		0x10, // #
		0x0e, //  ###
		0x00 //
	};
	disp.build_cc(4, cc2);
	char end [] = "Fin de la d\x04mo !";
	disp = end;
	disp << end;
	
	return 0;
}

/**
 * Fonction pour démo - bannière de texte.
 *
 * @param lcm		Référence vers un LCM
 * @param text		Texte à afficher (jeu de caractères de l'afficheur)
 * @param ms_interval	Intervalle en millisecondes entre chaque décallage
 */
static void banner(LCM& lcm, char* text, uint16_t ms_interval) {
	uint8_t start = 0, stop = 1, index = 0;
	uint16_t msg_len = cp_strlen(text);
	char t_char = 0;
	
	lcm.clear();
	cp_wait_ms(ms_interval);
	
	while (stop <= msg_len && start < msg_len - 1) {
		if (stop <= LCM_FW_HALF_CH) {
			index = LCM_FW_HALF_CH - stop;
		} else {
           	++start;
			index = 0;
		}
		
		t_char = text[stop];
		text[stop] = 0;
		lcm.write(text + start, index, stop == msg_len);
		text[stop] = t_char;

		if (stop != msg_len) {
			++stop;
		}

		cp_wait_ms(ms_interval);
	}
	
	lcm.clear();
	cp_wait_ms(ms_interval);
}

/**
 * Fonction pour démo - vague.
 *
 * @param lcm		Référence vers un LCM
 * @param rep		Nombre de répétitions
 * @param ms_interval	Intervalle en millisecondes entre chaque mouvement
 */
static void wave(LCM& lcm, uint16_t rep, uint16_t ms_interval) {
	uint8_t t_per [8];
	
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 8; ++j) {
			if (j == i) {
				t_per[j] = 0xff;
			} else {
				t_per[j] = 0;
			}
		}
		lcm.build_cc(i, t_per);
	}
	
	char custom_l1 [] = "\x20\x07\x05\x04\x03\x02\x02\x01\x01\x01\x02\x02"
		"\x03\x04\x05\x07\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x07\x05\x04\x03\x02\x02\x01\x01\x01"
		"\x02\x02\x03\x04\x05\x07";
	char custom_l2 [] = "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x08\x02\x03\x04\x05\x05\x06\x06\x06\x05"
		"\x05\x04\x03\x02\x08\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20";

	char t_char;
	
	lcm.clear();
	
	for (uint16_t k = 0; k < rep; ++k) {
		for (uint8_t i = 0; i < LCM_FW_TOT_CH; ++i) {
			t_char = custom_l1[i + 16];
			custom_l1[i + LCM_FW_HALF_CH] = 0;
			
			lcm.write(custom_l1 + i, 0);
			
			custom_l1[i + LCM_FW_HALF_CH] = t_char;
			t_char = custom_l2[i + LCM_FW_HALF_CH];
			custom_l2[i + LCM_FW_HALF_CH] = 0;
			
			lcm.write(custom_l2 + i, 16);
			
			custom_l2[i + LCM_FW_HALF_CH] = t_char;
			
			cp_wait_ms(ms_interval);
		}
	}
	
	lcm.clear();
}


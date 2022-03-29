// LIBRAIRIES
#include "rechercheDES.h"
#include "constantes.h"

// FONCTIONS
uint64_t cadencementCles(uint64_t K)
{
	uint64_t *sousCles = malloc(sizeof(uint64_t) * NOMBRE_TOURS);



	return sousCles;
}

uint64_t chiffrement(uint64_t clair, uint64_t K)
{
	/*"""m messages 64 bits, K la clé de 64 bits"""
    L = [0] * 16
    R = [0] * 16
    m_permuted = initial_permutation(m)
    L, R = m_permuted[:32], m_permuted[32:]
    subkeys = key_scheduling(K)

    for i in range(16):
        tmp = XOR(L, F(R, subkeys[i]))
        L = R
        R = tmp
    c = R + L # on inverse R et L a la fin
    c = final_permutation(c)
    return c*/

	uint64_t L = 0;
	uint64_t R = 0;
	uint64_t permute = permutationInitiale(clair);

	L = permute << (sizeof(uint64_t) - 32);
	R = permute >> (sizeof(uint64_t) - 32);

	uint64_t *sousCles = cadencementCles(K);

	free(sousCles);
}

uint64_t dechiffrement(uint64_t chiffre, uint64_t K)
{
	/*"""m messages 64 bits, K la clé de 64 bits"""
    L = [0] * 16
    R = [0] * 16
    m_permuted = initial_permutation(c)
    L, R = m_permuted[:32], m_permuted[32:]
    subkeys = key_scheduling(K)
    subkeys.reverse()

    for i in range(16):
        tmp = XOR(L, F(R, subkeys[i]))
        L = R
        R = tmp
    c = R + L # on inverse R et L a la fin
    c = final_permutation(c)
    return c*/
}

uint64_t permutationInitiale(uint64_t message)
{
	/*def initial_permutation(m):
    m_permuted = ""
    for i in range(64):
        m_permuted += m[IP[i] - 1]
    return m_permuted*/

	uint64_t permute = 0;

	for(int i = 0; i < TAILLE_BLOC; i++)
	{
		permute = message[IP[i] - 1];
	}

	return permute;
}

uint64_t permutationFinale(uint64_t message)
{
	/*c_final = ""
    for i in range(64):
        c_final += c[IP_inverse[i] - 1]
    return c_final*/

	uint64_t permute = 0;

	for(int i = 0; i < TAILLE_BLOC; i++)
	{
		permute = message[IPinverse[i] - 1];
	}

	return permute;
}

uint64_t expansion(uint64_t R)
{
	/*def expansion(R):
    R_48 = ""
    for i in range(48):
        R_48 += R[expansion_list[i] - 1]
    return R_48*/

	uint64_t R48 = 0;

	for(int i = 0; i < TAILLE_SOUS_CLE; i+)
	{
		R48 += R[E[i] - 1];
	}

	return R48;
}

uint64_t *classementSboxChiffresFautes(chiffre, chiffresFaux)
{
	uint64_t *classement = malloc(sizeof(uint64_t) * NOMBRE_SBOXES);

	/*liste_classement = [ [] for j in range(8) ]

    c_permuted = initial_permutation(c)
    R = c_permuted[32:]

    for i in range(len(liste_chiffres_fautes)):
        liste_sboxes = []
        cf_permuted = initial_permutation(hex_str_to_bin(liste_chiffres_fautes[i]))
        Rf = cf_permuted[32:]
        Re, Rfe = expansion(R), expansion(Rf)

        # e = XOR(R, Rf)
        e_expanded = XOR(Re, Rfe)
        for j in range(len(e_expanded)):
            if e_expanded[j] == '1':
                liste_classement[j // 6].append(i)

    return liste_classement*/

	for(int i = 0; i < NOMBRE_SBOXES; i++)
	{
		classement[i] = i;
	}

	uint64_t chiffrePermute = permutationInitiale(chiffre);
	uint64_t R = chiffrePermute << (sizeof(uint64_t) - 32);
	
	return classement;
}

uint64_t *bruteForceSboxes(uint64_t chiffre, uint64_t *chiffresFaux, uint64_t *classementSbox)
{
	uint64_t *clesCandidates;


	
	return clesCandidates;
}

uint64_t *choixClesCorrectes(uint64_t *clesCandidates)
{
	uint64_t *clesCorrectes;


	
	return clesCorrectes;
}

uint64_t concatene_ulint(uint64_t *clesCorrectes, int taille)
{
	uint64_t concatenation = 0;

	for(int i = 0; i < taille; i++)
	{
		concatenation += (clesCorrectes[i] << (i * TAILLE_PORTION_K16))
	}

	return concatenation;
}

uint64_t rechercheK16(uint64_t clair, uint64_t chiffre, uint64_t *chiffresFaux)
{
	uint64_t K16;

	/*
	list_potential_keys = []
    classement_sbox = classement_sbox_chiffres_fautes(c, liste_cf)
    liste_key_possible = brutforce_sboxes(c, liste_cf, classement_sbox)
    liste_key_juste_6 = choix_cle_juste(liste_key_possible)
    key_48 = ''.join(liste_key_juste_6)
    key_64 = brutforce_bits_manquants(m, c, key_48)
	*/

	unsigned int *classementSbox = classementSboxChiffresFautes(chiffre, chiffresFaux);
	uint64_t *clesCandidates = bruteForceSboxes(chiffre, chiffresFaux, classementSbox);
	uint64_t *clesCorrectes = choixClesCorrectes(clesCandidates);
	K16 = concatene_ulint(clesCorrectes, );

	free(classementSbox);
	free(clesCandidates);
	free(clesCorrectes);

	return K16;
}

uint64_t rechercheK(uint64_t clair, uint64_t chiffre, uint64_t K16)
{
	return 1;
}
# symmetric-encryption

## Contexte
- Projet de troisieme semestre, introduction à la cryptographie symmétrique en C ;
- Divisé en trois parties indépendantes mais complémentaires : Chiffrement ; Déchiffrement et Attaque .

## Breve Description
- Message clair est lu à partir d'un fichier passé en argument et chiffré dans un fichier destination ; 
- L'utilisateur peut choisir entre deux méthodes : chiffrement XOR (^) ou Cipher Block Chaining (CBC) ;
- Les fichiers peuvent être chiffrés mais aussi déchiffrés. 

- Ces deux méthodes étant vulnérable, on peut facilement 'deviner' les combinaisons de clés possibles vérifiant la visibilité des caractère ;
- Une analyse fréquentielle est réalisée sur le message crypté par rapport aux fréquences du langage cible.

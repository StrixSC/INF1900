

####Make file de la librairie####
####Pas encore complet. 
####Necessite des changements afin pouvoir compiler la librairie et l'exec dans une commande avec make,
####plutot que de toujours devoir changer de repertoire"

# Nom de votre projet
PROJECTNAME=libRobot

#Wildcard choisit de compiler tout les fichiers avec la terminaison specifiee.
#Dans notre cas, avec la terminaison "cpp"
PRJSRC= $(wildcard *.cpp)

#Cible que nous voulons creer a partir des commandes make. .a signifie un fichier archive
TRG=$(PROJECTNAME).a

all : $(TRG)

include ../Makefile_Common
#Createur de archive:
AR= avr-ar

$(TRG): $(OBJDEPS)
	$(AR) -crs $@ $^ 
	

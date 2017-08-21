#############################################################################
#  This file is part of the Stride software. 
#  It is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by 
#  the Free Software Foundation, either version 3 of the License, or any 
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#  Copyright 2017, Willem L, Kuylen E & Broeckhove J
#############################################################################
# IMMUNITY AND SUSCEPTIBILITY
#
# 
# Author: Lander Willem
# Last update: 18/08/2017  
#########################
rm(list=ls(all=TRUE))
gc()

tag <- '20170818'

############################################
## DATA FROM HENS et al. (2015) 	 	      ##
############################################


## TEMPORARY PROFILE
num_age <- 100
ages <- 0:(num_age-1)

suscept_profile <- ages*0
suscept_profile[1] <- 75
suscept_profile[2:9] <- 7 + ((1:8)^2)/20
suscept_profile[10:17] <- suscept_profile[9] - ((1:8)^2)/14
suscept_profile[18:20] <- suscept_profile[17] + ((1:3)^2)*1.8
suscept_profile[21:27] <- suscept_profile[20] - ((1:7)/1.5)
suscept_profile[28:29] <- suscept_profile[27] - ((1:2)*3)
suscept_profile[30:42] <- suscept_profile[29] - ((1:13)/2)
suscept_profile[43:num_age] <- suscept_profile[42] 

plot(suscept_profile,ylim=c(0,80),type='l')
 

immunity_profile <- round((100-suscept_profile)/100,digits=3)

immunity_profile_uniform <- immunity_profile
immunity_profile_uniform[] <- immunity_profile[80]
immunity_profile_uniform[1] <- immunity_profile[1]

immunity_profile_scare <- immunity_profile
immunity_profile_scare[2:7] <- 0.92-(1:6)*0.02
immunity_profile_scare[8:9] <- 0.8
immunity_profile_scare[10:15] <- 0.8+(1:6)*0.02
immunity_profile_scare[16:(num_age)] <- immunity_profile[2:(num_age-14)]

immunity_profile_regular <- immunity_profile_uniform
immunity_profile_regular[3:(num_age)] <- immunity_profile[2:(num_age-1)]

immunity_profile_catchup <- immunity_profile_regular
catchup_ages <- 20:38
catchup_fraq <- (immunity_profile[2]-immunity_profile_catchup[20:38])*0.8

immunity_profile_full <- immunity_profile
immunity_profile_full[] <- 1


immunity_profile_catchup[catchup_ages] <- immunity_profile_catchup[catchup_ages] + catchup_fraq
#immunity_profile_catchup[immunity_profile_catchup>0.99] <- 0.99


plot(immunity_profile,ylim=0:1,type='l')
lines(immunity_profile_uniform,col=2)
lines(immunity_profile_scare,col=3)
lines(immunity_profile_regular,col=4)
lines(immunity_profile_catchup,col=5)

abline(v=25)
abline(h=0.95,lty=2)
legend('bottomright',c('current','uniform','scare','regular','catchup'))

############################################
## SAVE AS XML  	 	                      ##
############################################

library(XML)

top = newXMLNode("immunity")
gen <- newXMLNode("current", parent = top)
for(i in 1:num_age){
  newXMLNode("proportion", immunity_profile[i], parent = gen)
}

gen <- newXMLNode("uniform", parent = top)
for(i in 1:num_age){
  newXMLNode("proportion", immunity_profile_uniform[i], parent = gen)
}

# gen <- newXMLNode("scare", parent = top)
# for(i in 1:num_age){
#   newXMLNode("proportion", immunity_profile_scare[i], parent = gen)
# }

gen <- newXMLNode("regular", parent = top)
for(i in 1:num_age){
  newXMLNode("proportion", immunity_profile_regular[i], parent = gen)
}

gen <- newXMLNode("catchup", parent = top)
for(i in 1:num_age){
  newXMLNode("proportion", immunity_profile_catchup[i], parent = gen)
}

gen <- newXMLNode("cocoon", parent = top)
for(i in 1:num_age){
  newXMLNode("proportion", immunity_profile_regular[i], parent = gen)
}

gen <- newXMLNode("full", parent = top)
for(i in 1:num_age){
  newXMLNode("proportion", immunity_profile_full[i], parent = gen)
}

top

saveXML(top, file="../resources/data/immunity_measels.xml",NewLineOnAttributes=T)
#saveXML(top, file=paste0(tag,"immunity_measels.xml"),NewLineOnAttributes=T)


sum(immunity_profile)/num_age

1300000/num_age*0.08
1300000*0.08

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
#
# VISUALIZE POPULATION IMMUNITY AND TRANSMISSION EVENTS
#
#############################################################################

rm(list=ls())

data_tag <- 'imm_current'
plot_immunity <- function(data_tag)
{
  
  
  io_folder <- '../' 

  # rename logfile => otherwise output of new run will be added
  orig_logfile <- paste0(io_folder,data_tag,'_logfile.txt')
  new_logfile <- paste0(io_folder,data_tag,'_logfile_fromR.txt')
  
  if(file.exists(orig_logfile)){
    file.rename(orig_logfile,new_logfile)
  }  
  
  getPDF <- TRUE
  
  ######################
  ## GET DATA         ##
  ######################
  print(data_tag)
  
  data_summary	     <- read.table(paste0(io_folder,data_tag,'_summary.csv',sep=''),header=TRUE,sep=",",stringsAsFactors=F)
  data_cases         <- read.table(paste(io_folder,data_tag,'_cases.csv',sep=''),header=FALSE,sep=",")
  data_person	       <- read.table(paste(io_folder,data_tag,'_person.csv',sep=''),header=TRUE,sep=",",stringsAsFactors=F)
  data_logfile       <- read.table(new_logfile,header=FALSE,sep=" ",stringsAsFactors=F)
     
  if(getPDF)
  pdf(paste0(io_folder,'plot_', data_tag,'_outbreaks.pdf'))
  
  names(data_person)
  
  popsize <- dim(data_person)[1]
  pop_age <- table(data_person$age)
  tbl_seroprev <- table(data_person$age,data_person$is_immune)

  # COMPARE WITH CURRENT PROFILE => EXTRA VACCINATION ?
  require(XML)
  data <- xmlParse('../data/disease_measles.xml')
  xml_data <- xmlToList(data)
  imm_subset <- xml_data[[7]]
  flag_reg    <- which(names(imm_subset) == 'current')
  age_imm_reg <- as.numeric(unlist(imm_subset[[flag_reg]]))
  
  age_imm_scenario <- tbl_seroprev[,2]/pop_age
  diff_imm_scenario <- age_imm_scenario-age_imm_reg

  par(mfrow=c(1,2))
  par(mar=c(5.1,4.1,2.1,4.1))
  num_ticks <- 4
  vacc_plot_ftr <- 5
  
  plot(matrix(tbl_seroprev[,2]/pop_age),main='age-specific immunity',xlab='age',ylab='seroprevalence',ylim=c(0,1),type='l')
  lines((diff_imm_scenario)*vacc_plot_ftr,col=3)
  axis(4,c(0:(num_ticks+1))*0.1*vacc_plot_ftr/(num_ticks+1),(0:(num_ticks+1))*0.1/(num_ticks+1),col.axis="green",col="green",cex.axis=0.8)
  abline(h=0.1,lty=3)
  abline(h=0.2,lty=3)
  abline(h=0.3,lty=3)
  mtext('Extra vaccination coverage', side=4, line=3, cex.lab=1, col="green")
  
  legend('right',c('extra vaccines',paste(round(sum(pop_age * (age_imm_scenario))- sum(pop_age * (age_imm_reg))))),cex=0.8)
  
  tbl_incidence <- table(data_person$age,data_person$is_recovered)
  tbl_incidence[85:100,2] <- NA
  y_max <- max(tbl_incidence[,2]/pop_age,0.01,na.rm=T)
  plot(matrix(tbl_incidence[,2]/pop_age),main='age-specific incidence',xlab='age',ylab='recovered',ylim=c(0,y_max),type='l')
  
  data_cases[1]
  plot_legend <- c(paste('prim. cases', (data_cases[1])),
                    paste('sec. cases', format(sum(data_person[,3]) - data_cases[1],digits=2)),
                    paste('attack rate', format(sum(data_person[,3]) / popsize,digits=2)),
                    paste('immunity', format(sum(data_person[,4]) / popsize,digits=4)),
                    paste('cases <1y', format(tbl_incidence[1,2],digits=0))
                   )
  legend('topleft',plot_legend,cex=1)
  
  par(mar=c(5.1,4.1,4.1,2.1))
  
  ## OUTBREAKS
  names(data_logfile) <- c('tag','infector_id','infected_id','cluster','day')
  
  cluster_levels <- c("household","school","work","primary_community","secondary_community")
  cluster_levels_proportion <- c(7,5,5,2,5)
  
  data_logfile$cluster_cat <- factor(data_logfile$cluster,cluster_levels)
  data_logfile[data_logfile==-1] <- NA
  
  infector <- table(data_logfile$infector_id)-1
  infector_id <- names(infector)
  
  barplot(table(infector),xlab='secundary cases',ylab='count',main='sec. cases',ylim=c(0,2000))
  barplot(table(infector)/length(infector),xlab='secundary cases',ylab='frequency',main='sec. cases',ylim=c(0,0.8))
  
  ## OUTBREAKS

  ## primary cases
  data_logfile$outbreak <- 0
  outbreak_id <- 1
  transmission_id <- NULL
  
  while(sum(data_logfile$outbreak ==0 ) > 0)
  {
    
    flag <- which(data_logfile$outbreak == 0)[1]
    infctrs <- data_logfile$infector_id[flag]
    num_infctrs_prev <- 0
    
    while(length(infctrs) > num_infctrs_prev)
    {
      num_infctrs_prev <- length(infctrs)
      case_flag <- data_logfile$infector_id %in% infctrs
      infctrs <- unique(c(infctrs,data_logfile$infected_id[case_flag]))
    }
    
    data_logfile$outbreak[data_logfile$infector_id %in% infctrs] <- outbreak_id
    transmission_id <- c(transmission_id,infctrs)
    outbreak_id <- outbreak_id + 1
  }
  
  # remove NA's
  transmission_id <- transmission_id[!is.na(transmission_id)]
  
  ## OUTBREAK SIZE
  tmp <- table(table(data_logfile$outbreak))
  tmp_size <- as.double(names(tmp))
  tmp_frac <- as.numeric(tmp/max(data_logfile$outbreak))
  
  plot_out <- matrix(ncol=max(tmp_size))
  # plot_out[tmp_size] <- tmp_frac
  plot_out[tmp_size] <- tmp
  plot_out <- as.numeric(plot_out)
  barplot(plot_out,xlab="outbreak size",main='outbreak size',ylab='fraction',ylim=c(0,2000))
  axis(1)
  legend('topright',c('total number',paste(sum(plot_out,na.rm=T))))
  
  
  ## OUTBREAK SIZE SELECTION
  plot_out <- as.numeric(plot_out[2:min(40,max(tmp_size))])
  barplot(plot_out,xlab="outbreak size",main='outbreak size (2-50)',ylab='fraction',ylim=c(0,500))
  axis(1)
  legend('topright',c('total number',paste(sum(plot_out,na.rm=T))))
  
  plot_out <- matrix(ncol=max(tmp_size))
  plot_out[tmp_size] <- tmp_frac
  plot_out <- as.numeric(plot_out)
  barplot(plot_out,xlab="outbreak size",main='outbreak size\nRELATIVE',ylab='fraction',ylim=c(0,1))
  axis(1)
  
  ## OUTBREAK SIZE SELECTION
  plot_out <- as.numeric(plot_out[2:min(40,max(tmp_size))])
  barplot(plot_out,xlab="outbreak size",main='outbreak size (2-50)\nRELATIVE',ylab='fraction',ylim=c(0,0.25))
  axis(1)
  
  
  ## CLUSTER
  
  plot_cluster_context <- function(flag,tag='')
  {
    tbl_cluster <- table(data_logfile$cluster_cat[flag])
    tbl_cluster_proportion <- tbl_cluster/cluster_levels_proportion
    barplot(tbl_cluster/sum(tbl_cluster),las=1,hor=T,cex.names=0.8,xlab='freq.',ylim=c(6,0.5),main=paste('transmission context\nTOTAL',tag),xlim=c(0,0.5))
    barplot(tbl_cluster_proportion/sum(tbl_cluster_proportion),las=1,hor=T,cex.names=0.8,xlab='freq.',ylim=c(6,0.5),main=paste('transmission context\nDAILY',tag),xlim=c(0,0.5))
    
  }
  
  # ALL
  plot_cluster_context(data_logfile$outbreak>-1)
  
  ## specific selections
  outbreak_size <- table(data_logfile$outbreak)
  selection <- names(outbreak_size[outbreak_size >= 2 & outbreak_size < 4])
  plot_cluster_context(data_logfile$outbreak %in% selection, '(2 <= size < 4)')
  
  selection <- names(outbreak_size[outbreak_size >= 4 & outbreak_size < 10])
  plot_cluster_context(data_logfile$outbreak %in% selection, '(4 <= size < 10)')
  
  selection <- names(outbreak_size[outbreak_size >= 10 ])
  plot_cluster_context(data_logfile$outbreak %in% selection, '(10 <= size)')
  
  par(mfrow=c(1,1))
  ## TRANSMISSION TREE
  if(length(transmission_id) < 5000)
  {
  
      transmission_id <- unique(transmission_id)
      num_days <- 150
      num_part <- length(transmission_id)
  
      plot(c(1,num_days),c(1,num_part),col=0,xlab='time (days)',ylab='individual',main='transmission tree')
      i_id <- data_logfile$infector_id[3]
      data_logfile$infected_id[is.na(data_logfile$infected_id)] <- -1
      for(i_id in data_logfile$infector_id)
      {
        flag      <- i_id == data_logfile$infector_id
        i_day     <- data_logfile$day[flag]
        i_case    <- data_logfile$infected_id[flag]
        i_cluster <- as.numeric(data_logfile$cluster_cat[flag])
          
        source <- which(i_id == transmission_id)
        prim_infection <- unique(data_logfile$day[i_id == data_logfile$infected_id])[1]                ### TODO!!!
        if(length(prim_infection) == 0)
        {
          prim_infection <- 0
        }
      
        for(i in 1:length(i_case))
        {
          
          if(i_case[i] == -1){
            points(c(prim_infection),c(source),type='p',pch=23,bg=1)
          } else {
            case <- which(!is.na(i_case[i]) & i_case[i] == transmission_id)
            day <- i_day[i]
            cluster <- i_cluster[i]
            
            lines(c(prim_infection,day),c(source,case),type='l',col=cluster)
            lines(c(prim_infection,day),c(source,case),type='b',col=cluster)
          }
          
        }
      }
      legend('bottomright',levels(data_logfile$cluster_cat),col=1:length(data_logfile$cluster_cat),lty=1)
  
    
    } # if num infected < 4000
  
  
  hist(data_person$start_symptomatic-data_person$start_infectiousness,xlab='days infectious, not symptomatic',main='')
  
  
  # location over time
  names(data_logfile)
  cluster_time <- table(data_logfile$cluster,data_logfile$day)
  cluster_time[cluster_time==0] <- NA
  plot_days <- as.double(colnames(cluster_time))
  plot(c(0,max(plot_days)),c(0,max(cluster_time,na.rm = T)),col=0,xlab='days',ylab='cases')
  
  i <- 1
  for(i in 1:dim(cluster_time)[1])
  lines(plot_days,cluster_time[i,],col=i,type='b',pch=20)
  legend('topleft',rownames(cluster_time),col=1:i,pch=20)
  
  
  if(getPDF)
  dev.off()

}

plot_immunity(data_tag)

# 
# plot_immunity('seroprev_current')
# plot_immunity('seroprev_catchup')
# plot_immunity('seroprev_cocoon')
# # #plot_immunity('scare')
# plot_immunity('seroprev_uniform')






 
 


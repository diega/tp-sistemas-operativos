#!/bin/bash
NO_ARGS=0 
E_OPTERROR=65
E_NOEXISTS=66

if [ $# -eq "$NO_ARGS" ]
then
  echo "Modo de uso: `basename $0` -s <ORIGEN> -d <DESTINO> -R"
  exit $E_OPTERROR
fi  

while getopts ":s:d:R" Option
do
  case $Option in
    s) 
      source_folder=$OPTARG
      if ! test -e $source_folder
      then 
        echo "El directorio $source_folder no existe"
        exit $E_NOEXISTS
      fi
    ;;
    d) 
      destination_folder=$OPTARG
      if ! test -e $destination_folder
      then 
        echo "+ -> $destination_folder "
        mkdir -p $destination_folder
      fi
    ;;
    R)
      recursive="-r"
  esac
done

if [ -z $source_folder ] || [ -z $destination_folder ]
then
  echo "Debe indicar carpeta de origen y destino. Ejecute el \\
        script sin parámetros para obtener más ayuda."
  exit $E_OPTERROR
fi
source_folder=$(echo $source_folder | sed "s/\/$//")
destination_folder=$(echo $destination_folder | sed "s/\/$//")
escaped_source_folder=$(echo $source_folder | sed "s/\//\\\\\//g") 

files_to_copy=`diff $recursive -q $source_folder $destination_folder \\
             | grep "Only in $source_folder" \\
             | sed -e "s/Only in $escaped_source_folder//" \\
             | sed -e "s/: /\//"`
for file in $files_to_copy
do
  echo "+ $source_folder$file -> $destination_folder$file"
  cp $recursive $source_folder$file $destination_folder
done

files_to_merge=`diff $recursive -q $source_folder $destination_folder \\
              | grep ".*differ" \\
              | sed -e "s/Files $escaped_source_folder//" \\
              | sed -e "s/ and .*//"`
for file in $files_to_merge
do
  if test $source_folder$file -nt $destination_folder$file
  then
    cp $recursive $source_folder$file $destination_folder
    echo "@ $source_folder$file -> $destination_folder$file"
  else
    echo "! $source_folder$file -- $destination_folder$file"
  fi
done

exit 0

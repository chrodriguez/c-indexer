#!/bin/bash

DIR=`dirname $0`
rm -fr $DIR/output
mkdir -p $DIR/output
for csv in $DIR/fixtures/*csv; do
  FILE=`basename $csv .csv`
  echo Genereting indexes for ${csv}...
  echo "  DNI: field 1"
  $DIR/../bin/indexer -i $csv -o $DIR/output/$FILE-dni.idx -f 1 -t number
  echo "  DATE: field 2"
  $DIR/../bin/indexer -i $csv -o $DIR/output/$FILE-date.idx -f 2 -t date
  echo "  SURNAME: field 3"
  $DIR/../bin/indexer -i $csv -o $DIR/output/$FILE-surname.idx -f 3 -t string
  echo "  FIRSTNAME: field 4"
  $DIR/../bin/indexer -i $csv -o $DIR/output/$FILE-firstname.idx -f 4 -t string
done


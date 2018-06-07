#!/usr/bin/env bash


RAW_LEADS_DIR=$1

OUTPUT_CSV_FILE="leadsOutput.csv"

LEAD_FILES=$(ls $RAW_LEADS_DIR)

# Create the file with the column headings
echo "Name,Company,Phone Number,Email" > $OUTPUT_CSV_FILE

echo "Here are all the 3rd lines"
for file in $LEAD_FILES; do
	THIRD_LINE=`sed -n '3p' $RAW_LEADS_DIR/$file`
	#echo $THIRD_LINE

	#CONTENT_PIECES=split(/ /, $THIRD_LINE)
	#IFS=', ' read -r -a CONTENT_PIECES <<< "$THIRD_LINE"

	# CONTENT_PIECES 0 is the "content" string. ignore it.
	CONTENT_PIECES=($THIRD_LINE)
	#echo "First element: ${CONTENT_PIECES[0]}"

	NAME="${CONTENT_PIECES[1]} ${CONTENT_PIECES[2]}"
	echo "Name: $NAME"
	EMAIL=${CONTENT_PIECES[-1]}
	echo "Email: $EMAIL"
	PHONE_NUMBER=${CONTENT_PIECES[-2]}
	echo "Phone Number: $PHONE_NUMBER"

	COMPANY_NAME_FINAL_INDEX=${#CONTENT_PIECES[@]}
	COMPANY_NAME_FINAL_INDEX=$((COMPANY_NAME_FINAL_INDEX-3))
	#echo "Final Name index: $COMPANY_NAME_FINAL_INDEX"

	for ((i=3;i<=COMPANY_NAME_FINAL_INDEX;i++))
	do
		#echo "Company word $i: ${CONTENT_PIECES[$i]}"
		COMPANY_NAME="$COMPANY_NAME ${CONTENT_PIECES[$i]}"
	done

	# Get rid of any commas in the name:
	COMPANY_NAME=$(echo $COMPANY_NAME | sed 's|,| |g')
	# Get rid of leading space
	#sed 's/^[[:space:]]*//' <<< "$COMPANY_NAME"
	echo "Company Name: $COMPANY_NAME"
	echo ""

	#### Ok, so now we we write this line to the csv file
	echo "$NAME,$COMPANY_NAME,$PHONE_NUMBER,$EMAIL" >> $OUTPUT_CSV_FILE
	COMPANY_NAME=""

done



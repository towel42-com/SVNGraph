#echo Updating base
#./createInclude.sh

echo Updating animations
cd animations && ../createInclude.sh

echo Updating areachart
cd ../areachart && ../createInclude.sh

echo Updating axis
cd ../axis && ../createInclude.sh

echo Updating domain
cd ../domain && ../createInclude.sh

echo Updating barchart
cd ../barchart && ../createInclude.sh

echo Updating legend
cd ../legend && ../createInclude.sh

echo Updating linechart
cd ../linechart && ../createInclude.sh

echo Updating piechart
cd ../piechart && ../createInclude.sh

echo Updating scatterchart
cd ../scatterchart && ../createInclude.sh

echo Updating splinechart
cd ../splinechart && ../createInclude.sh

echo Updating themese
cd ../themes && ../createInclude.sh

echo Updating xychart
cd ../xychart && ../createInclude.sh

echo Updating layout
cd ../layout && ../createInclude.sh

echo Updating boxplotchart
cd ../boxplotchart && ../createInclude.sh


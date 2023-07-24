import subprocess
from datetime import datetime
import requests
from bs4 import BeautifulSoup
from requests_html import HTMLSession

# Util Functions:
def subtractAndRound(num1, num2, digits=3):
    return round(num1 - num2, digits)

# Running Script Functions:
def runCommand(cmd, print_output = True):
    completed = subprocess.run(["powershell", "-Command", cmd], capture_output=True, text=True)

    if completed.returncode != 0:
        print("An error occurred: %s", completed.stderr)

    if (print_output):
        print(completed.stdout)

    return completed

def runMainScript(latitude, longitude, rightAscension, declination):
    completed = runCommand(f"./main.exe {latitude} {longitude} {rightAscension} {declination}")
    output_list = completed.stdout.split()
    calculated_altitude = float(output_list[-3])
    calculated_azimuth = float(output_list[-1])

    return {'altitude': calculated_altitude, 'azimuth': calculated_azimuth}

# Test Cases:
def moonTest(latitude, longitude):
    # get RA and Dec
    page = requests.get("https://theskylive.com/where-is-moon")
    soup = BeautifulSoup(page.content, "html.parser")

    elements = soup.find_all("div", {"class": "keyinfobox"})
    ra_element_list = [ item[:-1] for item in elements[0].get_text().split() ]
    dec_element_list = [ item[:-1] for item in elements[1].get_text().split() ]
    rightAscension = float(ra_element_list[2]) + (float(ra_element_list[3]) / 60) + (float(ra_element_list[4]) / 3600)
    declination = float(dec_element_list[1]) + (float(dec_element_list[2]) / 60) + (float(dec_element_list[3]) / 3600)

    # get actual altitude and azimuth
    page = requests.get(f"https://www.heavens-above.com/moon.aspx?lat={latitude}&lng={longitude}&loc=Del+Mar&alt=0&tz=PST")
    soup = BeautifulSoup(page.content, "html.parser")

    altitude = float(soup.find("span", {"id": "ctl00_cph1_lblAltitude"}).get_text()[:-1])
    azimuth = float(soup.find("span", {"id": "ctl00_cph1_lblAzimuth"}).get_text()[:-7])

    # recompile
    runCommand("make clean")
    runCommand("make")

    # run the script
    results = runMainScript(latitude, longitude, rightAscension, declination)
    calculated_altitude = results['altitude']
    calculated_azimuth = results['azimuth']


    # compare resulted values
    altitude_error = subtractAndRound(calculated_altitude, altitude)
    altitude_error_color = '🟩' if abs(altitude_error) < 5 else ('🟧' if abs(altitude_error) < 10 else '🟥')
    azimuth_error = subtractAndRound(calculated_azimuth, azimuth)
    azimuth_error_color = '🟩' if abs(azimuth_error) < 5 else ('🟧' if abs(azimuth_error) < 10 else '🟥')

    print(f"-----\n\nWhat values should be:\nAltitude: {altitude} (off by {altitude_error} {altitude_error_color})\nAzimuth: {azimuth} (off by {azimuth_error} {azimuth_error_color})")

def planetsTest(latitude, longitude):
    # get ra, dec, altitude, and azimuth for each planet
    page = requests.get(f"https://www.heavens-above.com/PlanetSummary.aspx?lat={latitude}&lng={longitude}&loc=Del+Mar&alt=0&tz=PST")
    soup = BeautifulSoup(page.content, "html.parser")

    planet_data = {}
    table = soup.find('table', attrs={'class':'standardTable'})

    table_head = [ val.text for val in table.find('thead').find('tr').find_all('td')[1:] ]
    for head in table_head:
        planet_data[head] = {}

    table_body_tr = table.find('tbody').find_all('tr')

    def getDecimalValues(index):
        return [ float(text_list[0][:-1]) + (float(text_list[1][:-1]) / 60) + (float(text_list[2][:-1]) / 3600) for text_list in [ val.text.split() for val in table_body_tr[index] ][1:] ]
    
    def removeDegreeSymbol(index):
        return [float(text) for text in [ val.text[:-1] for val in table_body_tr[index] ][1:]]

    ra_values = getDecimalValues(0)
    dec_values = getDecimalValues(1)
    altitude_values = removeDegreeSymbol(11)
    azimuth_values = removeDegreeSymbol(12)

    i = 0
    for key in planet_data.keys():
        planet_data[key]['ra'] = ra_values[i]
        planet_data[key]['dec'] = dec_values[i]
        planet_data[key]['altitude'] = altitude_values[i]
        planet_data[key]['azimuth'] = azimuth_values[i]

        i += 1

    # recompile
    runCommand("make clean")
    runCommand("make")

    # test each planet
    for key, value in planet_data.items():
        print(f"Running test for {key}: {value['ra']} {value['dec']}")
        results = runMainScript(latitude, longitude, value['ra'], value['dec'])

        planet_data[key]['calculated_altitude'] = results['altitude']
        planet_data[key]['calculated_azimuth'] = results['azimuth']

    # compare resulted values
    print("-----\n\nWhat values should be:")
    for key in planet_data.keys():
        altitude_error = subtractAndRound(planet_data[key]['calculated_altitude'], planet_data[key]['altitude']) 
        altitude_error_color = '🟩' if abs(altitude_error) < 5 else ('🟧' if abs(altitude_error) < 10 else '🟥')
        azimuth_error = subtractAndRound(planet_data[key]['calculated_azimuth'], planet_data[key]['azimuth'])
        azimuth_error_color = '🟩' if abs(azimuth_error) < 5 else ('🟧' if abs(azimuth_error) < 10 else '🟥')

        print(f"{key}:\n    Altitude: Actual — {planet_data[key]['altitude']}, Calculated — {planet_data[key]['calculated_altitude']} (off by {altitude_error} {altitude_error_color})\n    Azimuth: Actual — {planet_data[key]['azimuth']}, Calculated — {planet_data[key]['calculated_azimuth']} (off by {azimuth_error} {azimuth_error_color})")

if __name__ == '__main__':
    latitude = 32
    longitude = -117

    #moonTest(latitude, longitude)
    planetsTest(latitude, longitude)
    
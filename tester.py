import subprocess
from datetime import datetime
import requests
from bs4 import BeautifulSoup
from requests_html import HTMLSession

def run(cmd, print_output = True):
    completed = subprocess.run(["powershell", "-Command", cmd], capture_output=True, text=True)

    if completed.returncode != 0:
        print("An error occurred: %s", completed.stderr)

    if (print_output):
        print(completed.stdout)

    return completed

if __name__ == '__main__':
    latitude = 32
    longitude = -117

    local_time = datetime.now()
    year = local_time.year
    month = local_time.month
    day = local_time.day
    hour = local_time.hour
    minute = local_time.minute
    second = local_time.second

    # get RA and Dec
    page = requests.get("https://theskylive.com/where-is-moon")
    soup = BeautifulSoup(page.content, "html.parser")

    elements = soup.find_all("div", {"class": "keyinfobox"})
    ra_element_list = [ item[:-1] for item in elements[0].get_text().split() ]
    rightAscension = float(ra_element_list[2]) + (float(ra_element_list[3]) / 60) + (float(ra_element_list[4]) / 3600)
    declination = float(elements[1].get_text().split()[2][:-1])

    # get actual altitude and azimuth
    page = requests.get(f"https://www.heavens-above.com/moon.aspx?lat={latitude}&lng={longitude}&loc=Del+Mar&alt=0&tz=PST")
    soup = BeautifulSoup(page.content, "html.parser")

    altitude = float(soup.find("span", {"id": "ctl00_cph1_lblAltitude"}).get_text()[:-1])
    azimuth = float(soup.find("span", {"id": "ctl00_cph1_lblAzimuth"}).get_text()[:-6])

    # recompile
    run("make clean")
    run("make")

    completed = run(f"./main.exe {longitude} {latitude} {rightAscension} {declination} {year} {month} {day} {hour} {minute} {second}")
    calculated_altitude = float(completed.stdout.split()[18])
    calculated_azimuth = float(completed.stdout.split()[20])

    print(f"-----\n\nWhat values should be:\nAltitude: {altitude} (off by {altitude - calculated_altitude})\nAzimuth: {azimuth} (off by {azimuth - calculated_azimuth})")
'''
Outline:
This file contains a system for detecting
objects based on an array of distance values in
taken in a circle.'''
# import tabulate

class Object:
    '''Very unfinished. Just holds the number of rays wide
    the object is'''

    def __init__(self, start_index, end_index) -> None:
        self.start_index: int = start_index
        self.end_index: int = end_index
        self.width: int = end_index - start_index + 1

    def __repr__(self) -> str:
        return f'Object: Start: {round(self.start_index, 3)}, End: {round(self.end_index, 3)}, Width: {round(self.width, 3)}'


def cleanData(sample: list[int]) -> list[int]:
    from collections import Counter

    items = Counter(sample).items()
    for value, freq in sorted(items, reverse=True):
        if freq / len(sample) > 0.1: # frequent enough to be a true background value
            max_value = value
            break

    return [min(value, max_value) for value in sample]



def getKPointAverages(distance_values: list[int], k: int, background_value: int) -> list[int]:
    '''Returns a list of the k point averages of the given data'''
    # pad data with background values to allow for trailing k point average
    distance_values = [background_value]*(k-1) + distance_values

    return [sum(distance_values[i-k:i]) / k for i in range(k, len(distance_values)+1)]



def displayObjects(objects: list[Object]) -> None:
    '''Lists the objects in tidy rows and columns'''
    print('Index     Start      End        Width')

    for idx, obj in enumerate(objects):
        rounded_start: str = str(round(obj.start_index, 3))
        rounded_end: str = str(round(obj.end_index, 3))
        rounded_width: str = str(round(obj.width, 3))

        print(f'{str(idx):10s}{rounded_start:10s} {rounded_end:10s} {rounded_width:10s}')



def detectObjects(distances: list[int], k=5, print_data=False) -> list[Object]:
    '''Takes an array of distance values and detects significance
    changes in distances to signify objects.'''
    cleaned_dists: list[int] = cleanData(distances)

    background_dist: int = max(cleaned_dists)
    n: int = len(cleaned_dists)
    print(n)

    k_point_averages: list[int] = getKPointAverages(cleaned_dists, k, background_dist)

    # Initalise variables
    previous_average: int = k_point_averages[0]
    percent_change_threshold: int = 0.05 # arbitrary

    objects: list[Object] = []
    recording_object: bool = (previous_average - background_dist) / background_dist < percent_change_threshold
    object_start: int = 0


    for idx, average in enumerate(k_point_averages):
        percent_change: int = (average - previous_average) / previous_average

        if percent_change < -percent_change_threshold and not recording_object: # start recording new object
            object_start = idx
            recording_object = True

        elif percent_change > percent_change_threshold and recording_object: # stop recording object
            normalised_indices = (object_start / n * 360, (idx-1) / n * 360)
            # objects.append(Object(object_start, idx-1))
            objects.append(Object(*normalised_indices))
            recording_object = False

        if print_data:
            print(f'Idx: {idx}, Dist: {cleaned_dists[idx]}, %Change: {percent_change:.3f}, Detecting: {recording_object}')

        previous_average = average

    if recording_object: # add last object if it's cutoff
        objects.append(Object(object_start, idx))

    return objects

        

if __name__ == '__main__':
    from os import system; system('clear')

    # sample = [10]*30 + [2]*10 + [10]*50 + [3]*20 + [10]*100 + [1]*40 + [10]*110
    # sample = [10]*10 + [5]*10 + [10]*10 + [5]*10
    # sample = [10]*6 + [1]*6 + [10]*10

    # k = 5
    # averages = getKPointAverages(sample, 5, max(sample))
    # padded_sample = [max(sample)]*(k-1) + sample
    # print(' '.join(str(i).zfill(4) for i in range(-k+1, len(padded_sample)-k+1)))
    # print(' '.join(str(value).zfill(4) for value in padded_sample))
    # print(' '*4*5 + ' '.join(str(value).zfill(4) for value in averages))
    # print()

    with open('SonarData.csv') as file:
        lines = file.read().splitlines()
    sample = [int(line.split(',')[0]) for line in lines]


    objects = detectObjects(sample, k=5, print_data=False)
    displayObjects(objects)

    # for obj in objects:
    #     print(obj)

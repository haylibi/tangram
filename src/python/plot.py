# Connection to databaseG
import psycopg2 as psycopg
connection = psycopg.connect("dbname=postgis user=postgres password=postgres host=localhost port=5432")



# Plotting
import geopandas as gpd
import matplotlib
matplotlib.use('Qt5Agg')
import matplotlib.pyplot as plt

####### Plotting all the puzzles ###########
gdf = gpd.GeoDataFrame.from_postgis("SELECT * FROM puzzle", connection, geom_col='shape')
fig, axs = plt.subplots(ncols=gdf.shape[0], figsize=(8,8))
for i, row in gdf.iterrows():
    ax = axs[i]
    ax.set_title(row["name"])
    shape = row['shape']
    ax.set_aspect('equal')
    ax.set_axis_off()
    ax.fill(*shape.exterior.xy, color=row['color'])
    for interior in shape.interiors:
        ax.fill(*interior.xy, color='white')
plt.tight_layout()



# ####### Plotting all the pieces ###########
# gdf = gpd.GeoDataFrame.from_postgis( "SELECT * FROM piece", connection, geom_col='shape')
# fig, axs = plt.subplots(nrows=gdf.shape[0], figsize=(8,8))
# for i, row in gdf.iterrows():
#     ax = axs[i]
#     ax.set_title(f'{row["piece_type"]}')
#     shape = row['shape']
#     ax.set_aspect('equal')
#     ax.set_axis_off()
#     ax.fill(*shape.exterior.xy, color=row['color'])
#     ax.plot(*shape.exterior.xy, color='black', linewidth=1)
# plt.tight_layout()





####### Plotting all the solutions ###########
gdf = gpd.GeoDataFrame.from_postgis( "SELECT * FROM solution", connection, geom_col='shape')
fig, axs = plt.subplots(ncols=gdf.puzzle_name.nunique(), figsize=(8,8))
for (n, puzzle) in enumerate(gdf['puzzle_name'].unique().tolist()):
    ax = axs[n]
    ax.set_title(f'Solution - {puzzle}')
    ax.set_aspect('equal')
    ax.set_axis_off()
    for i, row in gdf[gdf.puzzle_name==puzzle].iterrows():
        shape = row['shape']
        ax.fill(*shape.exterior.xy, color=row['color'])
        ax.plot(*shape.exterior.xy, color='black', linewidth=1)

plt.tight_layout()
plt.show()



connection.close()
from setuptools import find_packages, setup

package_name = "mobile_robot_diagnostics"

setup(
    name=package_name,
    version="0.1.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
        ("share/" + package_name + "/launch", ["launch/diagnostics.launch.py"]),
        ("share/" + package_name + "/config", ["config/diagnostics.yaml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="arty2525",
    maintainer_email="arty2525@users.noreply.github.com",
    description="Runtime diagnostics สำหรับ Arty-ROS2 บน ROS 2 Jazzy",
    license="Apache-2.0",
    entry_points={"console_scripts": ["runtime_monitor = mobile_robot_diagnostics.runtime_monitor:main"]},
)

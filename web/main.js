document.querySelectorAll('.pallete div').forEach(pallete => {
    pallete.addEventListener('click', () => {
        const color = window.getComputedStyle(pallete).backgroundColor;
        console.log(color);
        localStorage.setItem('selectedColor', color);

        // Remove border from all palette divs
        document.querySelectorAll('.pallete div').forEach(p => {
            p.style.border = 'none';
        });

        // Add border to the selected palette div
        pallete.style.border = '3px solid black';
    });
});


document.addEventListener('DOMContentLoaded', () => {
    const mapPaths = document.querySelectorAll('.map path');
    const defaultColor = 'rgb(204, 204, 204)';  
    
    mapPaths.forEach(path => {
        path.addEventListener('click', () => {
            const selectedColor = localStorage.getItem('selectedColor') || defaultColor;
            path.style.fill = selectedColor;
        });
    });
});



document.querySelectorAll('.map path').forEach((path, index) => {
    path.id = (index + 1).toString();
});

function arePathsNear(path1, path2) {
    const rect1 = path1.getBoundingClientRect();
    const rect2 = path2.getBoundingClientRect();

    const isNear = !(rect1.right < rect2.left || 
                     rect1.left > rect2.right || 
                     rect1.bottom < rect2.top || 
                     rect1.top > rect2.bottom);

    return isNear;
}

function logNearbyPaths() {
    const paths = document.querySelectorAll('.map path');
    let logContent = '';

    const maxIndex = paths.length;
    logContent += `${maxIndex}\n\n`;

    paths.forEach((path, index) => {
        for (let i = index + 1; i < paths.length; i++) {
            if (arePathsNear(path, paths[i])) {
                logContent += `${index + 1} ${i + 1}\n`;
            }
        }
    });

    const blob = new Blob([logContent], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'nearby_paths_log.txt';
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
}

function applyColorsFromFile() {
    fetch('result.txt')
        .then(response => response.text())
        .then(data => {
            const lines = data.trim().split('\n');
            const colors = ['red', 'green', 'blue', 'yellow', 'purple', 'orange']; // Define your color palette here

            lines.forEach(line => {
                const [index, colorNumber] = line.split(' ').map(Number);
                const path = document.getElementById(index.toString());
                if (path && colors[colorNumber - 1]) {
                    path.style.fill = colors[colorNumber - 1];
                }
            });
        })
        .catch(error => console.error('Error reading the file:', error));
}

// document.addEventListener('DOMContentLoaded', applyColorsFromFile);
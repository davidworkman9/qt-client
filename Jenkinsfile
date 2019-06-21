#!groovy

pipeline {
    agent none
    stages {
        stage('Build on Linux') {
            agent { 
                label 'docker'
            }
            steps {
                sh label: 'Prep environment', script: 'cp ../work/jenkins.sh .'
                sh label: 'Build qt-client', script: "docker run -t --rm -v \$(pwd)/output:/root/output -v \$(pwd):/root/work xtbuilder5x-1804-512 /bin/bash /root/work/jenkins.sh"
                archiveArtifacts artifacts: 'output/*', fingerprint: true
            }
        }
    }
}

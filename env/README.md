# ENV: Developing in the Cloud

We assume you have followed the steps in [seafoodfry.github.io/aws/lab/2022/06/12/aws-lab-setup/](https://seafoodfry.github.io//aws/lab/2022/06/12/aws-lab-setup/).

However, you should not use `AWS-Vault`, instead you should use the [`Password AWS Shell Plugin](https://developer.1password.com/docs/cli/shell-plugins/aws/).
Make sure to add an MFA device!

The only reminder left is to remember to specify the profile you use for your IAM role.
In our case we will use
```
export AWS_PROFILE=ec2
```

We also recommend you use [github.com/tfutils/tfenv](https://github.com/tfutils/tfenv) to manage the Terraform versions you will need.

For this example we did
```
tfenv init
tfenv install
tfenv use <version>
```

You'll also need

```
brew install jq
```

---

## Running TF


In a file called `tf.env` add
```
ROLE="op://Private/AWS user key/iam role"
```

```
op run --no-masking --env-file=tf.env -- printenv ROLE
```

```
aws sts assume-role --profile ec2 --duration-seconds 900 --role-arn $ROLE --role-session-name test
```

## Spining Up an EC2

Remember to run
```
terraform init
```
If this is your first time doing it.

Then,
```
./run-cmd-in-shell.sh terraform plan -out a.plan -var my_ip="x.x.x.x"
```
Enter your IP.

Then apply the plan

```
./run-cmd-in-shell.sh terraform apply a.plan
```

To clean up
```
./run-cmd-in-shell.sh terraform destroy -var my_ip="x.x.x.x"
```

You can find logs in
```
cat /var/log/cloud-init-output.log
```

---

## SSH Tips

To copy files
```
scp Makefile ubuntu@${EC2}:/home/ubuntu
```

---

## Jupyter Tips

There are ready to use images in
[quay.io/organization/jupyter](https://quay.io/organization/jupyter).
That repository is documented in
[jupyter-docker-stacks](https://jupyter-docker-stacks.readthedocs.io/en/latest/).

```
docker run -p 8888:8888 quay.io/jupyter/scipy-notebook:python-3.11
```

You can then view it locally by running
```
ssh -L 8888:127.0.0.1:8888 ubuntu@${EC2}
```

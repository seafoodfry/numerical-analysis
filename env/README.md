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

```
terraform init
terraform plan -out a.plan
```

Enter your IP.
